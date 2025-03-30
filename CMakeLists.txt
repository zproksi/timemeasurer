cmake_minimum_required(VERSION 3.19)

project(timemeasurerlib)

set(SOURCE_FILES
    timemeasurer.cpp
)
set(HEADER_FILES
    timemeasurer.h
)

#create static library
add_library(timemeasurer STATIC ${SOURCE_FILES} ${HEADER_FILES})

#optionally
target_include_directories(timemeasurer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#optionally
set(TIMEMEASURER_VERSION 1.0.1)
