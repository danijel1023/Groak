cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 11)

project(stb)
add_library(stb STATIC "src/stb_image.cpp")
target_include_directories(stb PUBLIC "include")
