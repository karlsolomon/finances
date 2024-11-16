cmake_minimum_required(VERSION 3.30)
project(finances)
set(DEBUG false)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_LINKER "/usr/bin/g++")
include_directories(inc)
add_executable(
    finances
    src/main.cpp
    src/CsvParser.cpp
    src/Debt.cpp
    src/Worker.cpp
)
if (DEBUG)
    target_compile_options(
        finances
        PRIVATE -O0
                -g
                -Wall
                -Wextra
                -pedantic
                -Werror
    )
else ()
    target_compile_options(finances PRIVATE -O3)
endif ()
