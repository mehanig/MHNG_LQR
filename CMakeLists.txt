set(CMAKE_C_COMPILER "gcc-6")
set(CMAKE_CXX_COMPILER "gcc-6")


cmake_minimum_required(VERSION 3.6)
project(LQR)

set(CMAKE_CXX_STANDARD 14)

# Allow CLion read from current dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

#find_package( Threads )


SET(CMAKE_CXX_FLAGS_DEBUG "-O2")
SET(CMAKE_C_FLAGS_DEBUG "-O2")

set(SOURCE_FILES main.cpp MHNG_math.cpp MHNG_math.h file_operations.cpp file_operations.h typedefs.h)
add_executable(LQR ${SOURCE_FILES})