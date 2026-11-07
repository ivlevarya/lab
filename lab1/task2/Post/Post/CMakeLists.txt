cmake_minimum_required(VERSION 3.10)
project(Post)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(Post main.cpp Command.h Command.cpp PostMachine.h PostMachine.cpp Tape.h Tape.cpp)

add_executable(PostTests tests/test_post.cpp Command.h Command.cpp PostMachine.h PostMachine.cpp Tape.h Tape.cpp
)

add_test(NAME RunAllTests COMMAND PostTests)

add_custom_target(RunUnitTests
    COMMAND ${CMAKE_CTEST_COMMAND}
        -C $<CONFIG>
        --output-on-failure
        -R "^RunAllTests$"
    DEPENDS PostTests
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Running unit tests before building the application..."
    VERBATIM
)