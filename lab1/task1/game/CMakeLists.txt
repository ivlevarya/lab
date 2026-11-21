cmake_minimum_required(VERSION 3.15)
project(Game)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(Game game/main.cpp game/Header.h game/game.cpp)

add_executable(GameTests game/test.cpp game/Header.h game/game.cpp)

add_test(NAME RunAllTests COMMAND GameTests)

find_program(OPENCPPCOVERAGE_EXE OpenCppCoverage)

if(OPENCPPCOVERAGE_EXE)
    file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/game" NATIVE_SOURCES_DIR)
    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/coverage_report" NATIVE_REPORT_DIR)

    add_custom_target(coverage
        COMMAND ${OPENCPPCOVERAGE_EXE}
            --sources=${NATIVE_SOURCES_DIR}
            --excluded_sources=*catch.hpp*
            --excluded_sources=*test.cpp*
            --excluded_sources=*main.cpp*
            --export_type=html:${NATIVE_REPORT_DIR}
            -- $<TARGET_FILE:GameTests>
        COMMAND cmd /c start "" "${NATIVE_REPORT_DIR}\\index.html"
        DEPENDS GameTests
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running tests and generating code coverage report..."
    )
else()
    message(STATUS "OpenCppCoverage not found. 'coverage' target will not be available.")
endif()

