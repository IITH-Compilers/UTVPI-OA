cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(utvpi-oa-fm LANGUAGES CXX)

include_directories(include)
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(oafm ${SOURCES})
