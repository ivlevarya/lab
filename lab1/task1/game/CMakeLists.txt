cmake_minimum_required(VERSION 3.10)
project(Game)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(Game game/main.cpp game/Header.h game/game.cpp)

add_executable(GameTests game/test.cpp game/Header.h game/game.cpp)

add_test(NAME RunAllTests COMMAND GameTests)

