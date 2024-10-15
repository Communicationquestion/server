cmake_minimum_required(VERSION 3.8)

if(MSVC)
    add_compile_options(/utf-8)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(server LANGUAGES CXX)
set(Boost_INCLUDE_DIRS /usr/include/boost)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(server)
add_subdirectory(http)
add_subdirectory(thpool)