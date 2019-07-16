cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)

project(utvpi-oa-fm LANGUAGES CXX)

set(CPLEX_PATH "$ENV{HOME}/ibm/ILOG/CPLEX_Studio128" CACHE PATH "CPLEX Path")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-ignored-attributes")

include_directories(include)
include_directories("${CPLEX_PATH}/cplex/include")
include_directories("${CPLEX_PATH}/concert/include")

add_compile_definitions(IL_STD)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(utvpi-oa ${SOURCES})
target_link_libraries(utvpi-oa ${CPLEX_PATH}/cplex/lib/x86-64_linux/static_pic/libilocplex.a)
target_link_libraries(utvpi-oa ${CPLEX_PATH}/concert/lib/x86-64_linux/static_pic/libconcert.a)
target_link_libraries(utvpi-oa ${CPLEX_PATH}/cplex/lib/x86-64_linux/static_pic/libcplex.a)
target_link_libraries(utvpi-oa -lm -lpthread -ldl)
