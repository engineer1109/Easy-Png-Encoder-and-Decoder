cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

add_definitions(-DPNG_DEMO_DATA_DIR="${PROJECT_SOURCE_DIR}/data")

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(OpenCV)

add_executable(PngCodecTest main.cxx)

target_link_libraries(PngCodecTest PngCodec ${OpenCV_LIBRARIES})
