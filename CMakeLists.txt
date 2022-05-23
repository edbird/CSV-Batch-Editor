set(CMAKE_C_COMPILER /usr/local/inferno/copyleft/gcc-9.2.0/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/local/inferno/copyleft/gcc-9.2.0/bin/g++)
set(ENV{LD_LIBRARY_PATH} /usr/local/inferno/copyleft/gcc-9.2.0/:ENV{LD_LIBRARY_PATH})

cmake_minimum_required(VERSION 3.16)

project(csvbatcheditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(a.out main.cpp)