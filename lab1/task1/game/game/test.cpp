#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Header.h"
#include <iostream>
#include <sstream>
#include <string>

TEST_CASE("Moves a tile to the empty position if the move is valid.") {
	Game a;
	REQUIRE(a.Change(3, 2) == true);
	REQUIRE(a.Change(0, 0) == false);
}

TEST_CASE("Get number of tile.") {
	Game a;
	REQUIRE(a.GetNum(3, 3) == 0);
	REQUIRE(a.GetNum(0, 0) == 1);
}
TEST_CASE("Compare two tiles.") {
	Game a;
	Game b;
	a.Change(3, 2);
	REQUIRE((a == b) == false);
	REQUIRE((a != b) == true);
}
TEST_CASE("Shaker move empty tile.") {
	Game a;
	Game b;
	a.Shaker(200);
	REQUIRE((a == b) == false);
}
TEST_CASE("Initial board contains all tiles in the correct order") {
    Game game;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int expected = (i == 3 && j == 3)
                ? 0
                : i * 4 + j + 1;

            REQUIRE(game.GetNum(i, j) == expected);
        }
    }
}

TEST_CASE("The empty cell can move in all four directions") {
    Game game;

    // Up
    REQUIRE(game.Change(2, 3));
    REQUIRE(game.GetNum(2, 3) == 0);
    REQUIRE(game.GetNum(3, 3) == 12);

    // Left
    REQUIRE(game.Change(2, 2));
    REQUIRE(game.GetNum(2, 2) == 0);
    REQUIRE(game.GetNum(2, 3) == 11);

    // Down
    REQUIRE(game.Change(3, 2));
    REQUIRE(game.GetNum(3, 2) == 0);
    REQUIRE(game.GetNum(2, 2) == 15);

    // Right
    REQUIRE(game.Change(3, 3));
    REQUIRE(game.GetNum(3, 3) == 0);
    REQUIRE(game.GetNum(3, 2) == 12);
}

TEST_CASE("Invalid moves do not change the board") {
    Game game;
    Game original;

    SECTION("Negative row") {
        REQUIRE_FALSE(game.Change(-1, 3));
    }

    SECTION("Row beyond the board") {
        REQUIRE_FALSE(game.Change(4, 3));
    }

    SECTION("Negative column") {
        REQUIRE_FALSE(game.Change(3, -1));
    }

    SECTION("Column beyond the board") {
        REQUIRE_FALSE(game.Change(3, 4));
    }

    SECTION("Diagonal tile") {
        REQUIRE_FALSE(game.Change(2, 2));
    }

    SECTION("Distant tile") {
        REQUIRE_FALSE(game.Change(0, 0));
    }

    SECTION("The empty cell itself") {
        REQUIRE_FALSE(game.Change(3, 3));
    }

    REQUIRE((game == original));
}

TEST_CASE("Comparison detects equal and different boards") {
    Game a;
    Game b;

    // Initially equal
    REQUIRE((a == b));
    REQUIRE_FALSE((a != b));

    // Different after moving a tile in one board
    REQUIRE(a.Change(3, 2));
    REQUIRE_FALSE((a == b));
    REQUIRE((a != b));

    // Equal again after making the same move
    REQUIRE(b.Change(3, 2));
    REQUIRE((a == b));
    REQUIRE_FALSE((a != b));
}

TEST_CASE("Draw board prints tiles and the empty cell") {
    Game game;
    Game original;

    std::ostringstream output;

    std::streambuf* oldBuffer = std::cout.rdbuf(output.rdbuf());

    drawBoard(game);

    std::cout.rdbuf(oldBuffer);

    const std::string expected =
        "  1|  2|  3|  4|\n"
        "----------------\n"
        "  5|  6|  7|  8|\n"
        "----------------\n"
        "  9| 10| 11| 12|\n"
        "----------------\n"
        " 13| 14| 15|  *|\n"
        "----------------\n";

    REQUIRE(output.str() == expected);
    REQUIRE((game == original));
}