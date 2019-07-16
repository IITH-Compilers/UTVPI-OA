cmake_minimum_required(VERSION 3.12)
project(UTVPI-OA)

set(PIPLIB_PATH "${CMAKE_SOURCE_DIR}/../pocc-1.4.2/math/install-piplib" CACHE PATH "PIPLib Path")

include_directories(${FMLIB_PATH}/include/)
file(GLOB SOURCES src/*.c)

add_executable(lp-pip ${SOURCES})
target_link_libraries(lp-pip -L${FMLIB_PATH}/lib -lpiplib64)
