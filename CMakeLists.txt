cmake_minimum_required(VERSION 3.17)
project(AliciaCPAK)

set(CMAKE_CXX_STANDARD 20)

add_executable(AliciaCPAK source/main.cpp source/buffer/buffer.hpp source/buffer/buffer.cpp source/file/pak.cpp source/file/pak.hpp)
include_directories(source)