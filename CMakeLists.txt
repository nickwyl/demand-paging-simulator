cmake_minimum_required(VERSION 3.16)
project(demand-paging-simulator)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++0x")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")

file (GLOB source_files *.cpp)

add_executable (demand-paging-simulator src/main.cpp src/Page.cpp src/Process.cpp src/Page.h src/Process.h)
