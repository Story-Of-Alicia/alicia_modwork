cmake_minimum_required(VERSION 3.17)
project(barehands)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(${PROJECT_NAME} Boost::program_options)
add_executable(barehands source/main.cpp source/buffer/buffer.hpp source/buffer/buffer.cpp source/file/pak.cpp source/file/pak.hpp)
include_directories(source/)
include_directories(deps/include)

target_link_libraries(barehands ${CMAKE_SOURCE_DIR}/deps/lib/zlib.a)