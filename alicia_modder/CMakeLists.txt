cmake_minimum_required(VERSION 3.19)
project(alicia_modder)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if (NOT EXISTS "${CMAKE_BINARY_DIR}/cmake/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/develop/conan.cmake"
            "${CMAKE_BINARY_DIR}/cmake/conan.cmake"
            TLS_VERIFY ON)
endif ()

include(${CMAKE_BINARY_DIR}/cmake/conan.cmake)
conan_cmake_configure(REQUIRES spdlog/1.8.5 ctre/3.4.1 cxxopts/2.2.1 imgui/1.83 zlib/1.2.11
        GENERATORS cmake_find_package)
conan_cmake_autodetect(settings)
conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings})

find_package(spdlog)
find_package(imgui)
find_package(ctre)
find_package(cxxopts)
find_package(ZLIB)
link_libraries(spdlog::spdlog ctre::ctre cxxopts::cxxopts imgui::imgui d3d9 ZLIB::ZLIB)

set(AM_VENDOR_SOURCES
        vendor/imgui/imgui_impl_win32.h
        vendor/imgui/imgui_impl_win32.cpp
        vendor/imgui/imgui_impl_dx9.h
        vendor/imgui/imgui_impl_dx9.cpp
        )

set(AM_SOURCES
        source/main/buffer/buffer.cpp
        source/main/buffer/buffer.hpp
        source/main/main.cpp

        source/main/mod/alicia/game/alicia_modder.cpp
        source/main/mod/alicia/game/alicia_modder.hpp

        source/main/interface/interface.hpp
        source/main/interface/interface.cpp

        source/main/tools/dbg/debugger.hpp
        source/main/tools/dbg/debugger.cpp

        source/main/mod/alicia/launcher/launcher_modder.cpp
        source/main/mod/alicia/launcher/launcher_modder.hpp

        source/main/interface/views/data_view.cpp
        source/main/interface/views/data_view.hpp

        source/main/tools/assets/defs.hpp

        source/main/tools/rw/defs.hpp source/main/tools/assets/assets.cpp source/main/tools/assets/assets.hpp source/main/helpers/filesystem.cpp source/main/helpers/filesystem.hpp source/main/mod/alicia/assets/assets_modder.cpp source/main/mod/alicia/assets/assets_modder.hpp)

include_directories(vendor/)
include_directories(source/)

add_executable(alicia_modder ${AM_VENDOR_SOURCES} ${AM_SOURCES})