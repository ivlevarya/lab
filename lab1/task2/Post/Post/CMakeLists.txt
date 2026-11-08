cmake_minimum_required(VERSION 3.10)
project(Post)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(Post Post/main.cpp Post/Command.h Post/Command.cpp Post/PostMachine.h Post/PostMachine.cpp Post/Tape.h Post/Tape.cpp)

add_executable(PostTests Post/tests.cpp Post/Command.h Post/Command.cpp Post/PostMachine.h Post/PostMachine.cpp Post/Tape.h Post/Tape.cpp
)


add_test(NAME RunAllTests COMMAND PostTests)

add_custom_target(RunUnitTests
    COMMAND ${CMAKE_CTEST_COMMAND}
        -C $<CONFIG>
        --output-on-failure
    COMMENT "Running unit tests before building the application..."
)