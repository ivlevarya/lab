#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "PostMachine.h"

#include <cstdio>
#include <iostream>
#include <sstream>


namespace {

    struct File {
        std::string path;

        explicit File(const std::string& text) {
            static unsigned id = 0;
            path = "post_test_fixture_" + std::to_string(++id) + ".txt";
            std::ofstream out(path);
            out << text;
        }

        ~File() {
            std::remove(path.c_str());
        }
    };

    struct Streams {
        std::istringstream in;
        std::ostringstream out, err;
        std::streambuf* oldIn, * oldOut, * oldErr;

        explicit Streams(const std::string& input = "")
            : in(input),
            oldIn(std::cin.rdbuf(in.rdbuf())),
            oldOut(std::cout.rdbuf(out.rdbuf())),
            oldErr(std::cerr.rdbuf(err.rdbuf())) {
            std::cin.clear();
        }

        ~Streams() {
            std::cin.rdbuf(oldIn);
            std::cout.rdbuf(oldOut);
            std::cerr.rdbuf(oldErr);
            std::cin.clear();
        }
    };

    std::string listing(PostMachine& pm) {
        Streams io;
        pm.printProgram();
        return io.out.str();
    }

} // namespace

TEST_CASE("Tape initialization and execution lifecycle") {
    PostMachine pm;

    REQUIRE(pm.isStopped());
    REQUIRE(pm.getCells() == std::vector<int>{0});
    REQUIRE(pm.getCarriage() == 0);

    pm.setTape({}, 0);
    REQUIRE(pm.getCells() == std::vector<int>{0});

    pm.start();
    REQUIRE(pm.isStopped());

    pm.prepareExecution();
    REQUIRE(pm.isStopped());

    pm.setProgram({ Command('V', 2), Command('!') });
    pm.executeStep();
    REQUIRE(pm.getCells()[0] == 0);

    pm.prepareExecution();
    REQUIRE_FALSE(pm.isStopped());

    pm.executeStep();
    REQUIRE(pm.getCells()[0] == 1);
    REQUIRE_FALSE(pm.isStopped());

    pm.stop();
    pm.executeStep();
    REQUIRE(pm.isStopped());

    pm.setTape({ 0 });
    pm.start();
    REQUIRE(pm.getCells()[0] == 1);
    REQUIRE(pm.isStopped());

    pm.setProgram({ Command('!') });
    REQUIRE(pm.isStopped());

    pm.prepareExecution();
    pm.addCommand(Command('!'));
    REQUIRE(pm.isStopped());
}

TEST_CASE("Movement preserves cells and expands tape at either end") {
    PostMachine pm;
    pm.setTape({ 1, 0 }, 1);
    pm.setProgram({
        Command('<', 2),
        Command('>', 3),
        Command('>', 4),
        Command('!')
        });

    pm.start();
    REQUIRE(pm.getCells() == std::vector<int>{1, 0, 0});
    REQUIRE(pm.getCarriage() == 2);

    pm.setTape({ 1 });
    pm.setProgram({ Command('<', 2), Command('!') });
    pm.start();

    REQUIRE(pm.getCells() == std::vector<int>{0, 1});
    REQUIRE(pm.getCarriage() == 0);
}

TEST_CASE("Marks and conditional jumps") {
    PostMachine pm;

    SECTION("Set and erase") {
        pm.setProgram({
            Command('V', 2), Command('X', 3), Command('!')
            });
        pm.start();
        REQUIRE(pm.getCells() == std::vector<int>{0});
    }

    SECTION("Writing an existing mark stops without advancing") {
        pm.setTape({ 1 });
        pm.setProgram({ Command('V', 2), Command('>', 3) });
        pm.start();
        REQUIRE(pm.getCarriage() == 0);
        REQUIRE(pm.getCells()[0] == 1);
    }

    SECTION("Erasing a blank stops without advancing") {
        pm.setProgram({ Command('X', 2), Command('>', 3) });
        pm.start();
        REQUIRE(pm.getCarriage() == 0);
        REQUIRE(pm.getCells()[0] == 0);
    }

    SECTION("Blank branch") {
        pm.setProgram({
            Command('?', 2, 3), Command('V', 3), Command('!')
            });
        pm.start();
        REQUIRE(pm.getCells()[0] == 1);
    }

    SECTION("Marked branch") {
        pm.setTape({ 1 });
        pm.setProgram({
            Command('?', 2, 3), Command('>', 3),
            Command('X', 4), Command('!')
            });
        pm.start();
        REQUIRE(pm.getCarriage() == 0);
        REQUIRE(pm.getCells()[0] == 0);
    }

    SECTION("Nonbinary cell") {
        pm.setTape({ 2 });
        pm.setProgram({ Command('?', 2, 3), Command('>', 3) });
        pm.start();
        REQUIRE(pm.getCells()[0] == 2);
        REQUIRE(pm.getCarriage() == 0);
    }

    REQUIRE(pm.isStopped());
}

TEST_CASE("Invalid command, carriage and jump stop safely") {
    PostMachine pm;

    SECTION("Unknown symbol") {
        pm.setProgram({ Command('@') });
    }
    SECTION("Negative carriage") {
        pm.setTape({ 0 }, -1);
        pm.setProgram({ Command('V') });
    }
    SECTION("Carriage beyond tape") {
        pm.setTape({ 0 }, 1);
        pm.setProgram({ Command('V') });
    }
    SECTION("Zero jump") {
        pm.setProgram({ Command('>', 0) });
    }
    SECTION("Negative jump") {
        pm.setProgram({ Command('>', -1) });
    }
    SECTION("Jump beyond program") {
        pm.setProgram({ Command('>', 99) });
    }

    pm.start();
    REQUIRE(pm.isStopped());
}

TEST_CASE("Stopped command is a no-op") {
    Tape tape;
    int current = 7;
    bool stopped = true;

    Command('V', 2).execute(tape, current, stopped);

    REQUIRE(current == 7);
    REQUIRE(stopped);
}

TEST_CASE("Program listing and deletion") {
    PostMachine pm;
    REQUIRE_FALSE(listing(pm).empty());
    REQUIRE_FALSE(pm.deleteCommand(1));

    pm.setProgram({
        Command('?', 3, 4), Command('V', 4),
        Command('>', 4), Command('!')
        });

    REQUIRE(listing(pm) == "1: ?-3-4\n2: V-4\n3: >-4\n4: !\n");
    REQUIRE_FALSE(pm.deleteCommand(0));
    REQUIRE_FALSE(pm.deleteCommand(-1));
    REQUIRE_FALSE(pm.deleteCommand(5));
    REQUIRE_FALSE(pm.deleteCommand(3));
    REQUIRE_FALSE(pm.deleteCommand(4));

    REQUIRE(pm.deleteCommand(2));
    REQUIRE(listing(pm) == "1: ?-2-3\n2: >-3\n3: !\n");
    REQUIRE(pm.isStopped());

    pm.setProgram({ Command('?', 1, 2), Command('!') });
    REQUIRE_FALSE(pm.deleteCommand(2));

    pm.setProgram({
        Command('!', 99), Command('V', 1), Command('!', 1)
        });
    REQUIRE(pm.deleteCommand(2));
    REQUIRE(listing(pm) == "1: !\n2: !\n");

    pm.setProgram({ Command('V', 1) });
    REQUIRE(pm.deleteCommand(1));
    pm.start();
    REQUIRE(pm.isStopped());

    pm.setProgram({ Command('V', 1), Command('!', 1) });
    REQUIRE(pm.deleteCommand(2));
    REQUIRE(listing(pm) == "1: V-1\n");
}

TEST_CASE("Load valid programs and blank lines") {
    const auto input = GENERATE(
        std::string("010\n2\nV-2\n!\n"),
        std::string("010\n2\n\n\t\r\nV-2\n!\n")
    );

    File fixture(input);
    std::ifstream file(fixture.path);
    PostMachine pm;

    REQUIRE(pm.load(file, pm));
    REQUIRE(pm.getCells() == std::vector<int>{0, 1, 0});

    // Characterizes the existing bug in !ss >> carriage.
    // The intended carriage index for input "2" would be 1.
    REQUIRE(pm.getCarriage() == -1);

    REQUIRE(pm.isStopped());
    REQUIRE(listing(pm) == "1: V-2\n2: !\n");
}

TEST_CASE("Load empty tape and all supported symbols") {
    auto tape = GENERATE(std::string(""), std::string("\t\r"));
    File fixture(tape + "\n1\n<-2\n>-3\nV-4\nX-5\n?-1-6\n!\n");
    std::ifstream file(fixture.path);
    PostMachine pm;

    REQUIRE(pm.load(file, pm));
    REQUIRE(pm.getCells() == std::vector<int>{0});
    REQUIRE(listing(pm) ==
        "1: <-2\n2: >-3\n3: V-4\n4: X-5\n5: ?-1-6\n6: !\n");
}

TEST_CASE("Load rejects invalid command syntax") {
    auto command = GENERATE(
        " ", "@", "V", "V:1", "V-0", "V--1",
        "?-1", "?-1:2", "?-1-0", "?-1--2"
    );

    File fixture(std::string("0\n1\n") + command + "\n");
    std::ifstream file(fixture.path);
    PostMachine pm;

    REQUIRE_FALSE(pm.load(file, pm));
}

TEST_CASE("Load closed file fails") {
    std::ifstream file;
    PostMachine pm;
    REQUIRE_FALSE(pm.load(file, pm));
}

TEST_CASE("Loader current behavior for missing or invalid carriage") {
    // Characterizes the existing precedence bug: !ss >> carriage.
    auto carriage = GENERATE("", "\t\r", "bad");
    File fixture(std::string("0\n") + carriage + "\n!\n");
    std::ifstream file(fixture.path);
    PostMachine pm;

    REQUIRE(pm.load(file,
        pm));
    REQUIRE(pm.getCarriage() ==
        (std::string(carriage) == "bad" ? -1 : 0));
}

