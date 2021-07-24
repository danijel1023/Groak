cmake_minimum_required (VERSION 3.8)

add_executable (Test_Application "main.cpp")
target_link_libraries(Test_Application PRIVATE "Groak")
target_include_directories(Test_Application PRIVATE "../include")
