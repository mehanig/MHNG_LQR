cmake_minimum_required(VERSION 3.6)
project(LQR)

set(CMAKE_CXX_STANDARD 14)

# Allow CLion read from current dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES main.cpp MHNG_math.cpp MHNG_math.h file_operations.cpp file_operations.h typedefs.h)
add_executable(LQR ${SOURCE_FILES})