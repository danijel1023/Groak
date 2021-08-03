cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 11)

project(glad)
add_library(glad STATIC "src/glad.c")
target_include_directories(glad PUBLIC "include")

target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})
