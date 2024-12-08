cmake_minimum_required(VERSION 3.30)
project(finances)
set(DEBUG true)
set(CMAKE_CXX_STANDARD 23) # TODO (ksolomon): change to C++17 for nvc++ compatibility
set(NV false)
if (NV)
    set(TOOLCHAIN_PATH "/opt/nvidia/hpc_sdk/Linux_x86_64/24.11/compilers/bin")
    set(COMPILER "nvc++")
else ()
    set(TOOLCHAIN_PATH "/usr/bin")
    set(COMPILER "g++")
endif ()
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/${COMPILER}")
set(CMAKE_CXX_LINKER "${TOOLCHAIN_PATH}/${COMPILER}")
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
target_link_libraries(finances PRIVATE tbb)
