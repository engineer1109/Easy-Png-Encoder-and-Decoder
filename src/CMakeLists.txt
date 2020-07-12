cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

add_definitions(-DLIGPNGCODEC_EXPORT_API)

include_directories(${PROJECT_SOURCE_DIR}/include)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)

FILE(GLOB_RECURSE HEAD_FILES ${PROJECT_SOURCE_DIR}/include/*.h)

add_library(PngCodec ${SRC_FILES} ${HEAD_FILES})

target_link_libraries(PngCodec ${PNG_LIBRARIES})