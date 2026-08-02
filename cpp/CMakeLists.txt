cmake_minimum_required(VERSION 3.20)
project(datastructures_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This points to your future code
add_executable(datastructures_cpp src/main.cpp)

# Warnings are on by default so a new one surfaces when it is introduced
# rather than much later. Clang and GCC syntax; this project targets macOS.
target_compile_options(datastructures_cpp PRIVATE -Wall -Wextra)
