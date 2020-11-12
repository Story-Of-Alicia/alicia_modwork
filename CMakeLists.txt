cmake_minimum_required(VERSION 3.17)
project(barehands)

set(CMAKE_CXX_STANDARD 20)

add_executable(barehands source/main.cpp source/buffer/buffer.hpp source/buffer/buffer.cpp source/file/pak.cpp source/file/pak.hpp)
include_directories(source/)
include_directories(deps/include)
target_link_libraries(barehands ${CMAKE_SOURCE_DIR}/deps/lib/libz)