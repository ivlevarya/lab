cmake_minimum_required(VERSION 3.10)
project(Game)

enable_testing()

add_executable(Game game/main.cpp game/Header.h game/game.cpp)

add_executable(GameTests game/test.cpp game/Header.h game/game.cpp)

add_test(NAME RunAllTests COMMAND GameTests)

add_custom_command(
    TARGET Game PRE_BUILD
    COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIG> --output-on-failure
    COMMENT "Running unit tests before building the application..."
)