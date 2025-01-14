cmake_minimum_required(VERSION 3.20)

project(YohohoScript VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.hpp
)

add_executable(yhs ${SOURCES})

# Set the output directory for the executable
set_target_properties(yhs PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/output"
)