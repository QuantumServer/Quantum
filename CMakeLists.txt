cmake_minimum_required(VERSION 3.1)

project(Quantum)

## add_subdirectory(lib/cppraklib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(include)
