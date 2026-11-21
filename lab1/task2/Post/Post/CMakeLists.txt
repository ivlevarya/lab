cmake_minimum_required(VERSION 3.15)
project(Post)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(Post Post/main.cpp Post/Command.h Post/Command.cpp Post/PostMachine.h Post/PostMachine.cpp Post/Tape.h Post/Tape.cpp)

add_executable(PostTests Post/tests.cpp Post/Command.h Post/Command.cpp Post/PostMachine.h Post/PostMachine.cpp Post/Tape.h Post/Tape.cpp
)


add_test(NAME RunAllTests COMMAND PostTests)

find_program(OPENCPPCOVERAGE_EXE OpenCppCoverage)

if(OPENCPPCOVERAGE_EXE)
    file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/Post" NATIVE_SOURCES_DIR)
    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/coverage_report" NATIVE_REPORT_DIR)

    add_custom_target(coverage
        COMMAND ${OPENCPPCOVERAGE_EXE}
            --sources=${NATIVE_SOURCES_DIR}
            --excluded_sources=*catch.hpp*
            --excluded_sources=*tests.cpp*
            --excluded_sources=*main.cpp*
            --export_type=html:${NATIVE_REPORT_DIR}
            -- $<TARGET_FILE:PostTests>
        COMMAND cmd /c start "" "${NATIVE_REPORT_DIR}\\index.html"
        DEPENDS PostTests
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running tests and generating code coverage report..."
    )
else()
    message(STATUS "OpenCppCoverage not found. 'coverage' target will not be available.")
endif()
