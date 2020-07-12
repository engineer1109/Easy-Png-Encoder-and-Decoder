cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(PngCodec)

if(WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_USE_MATH_DEFINES)
endif()

find_package(PNG)

add_subdirectory(src)
add_subdirectory(test)


