cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 17)

project(Groak VERSION 1.0)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_library(Groak STATIC ${SRC_FILES})

#build glad & glfw libraries
add_subdirectory("Vendor/glad")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("Vendor/glfw3")


add_compile_definitions(GLEW_STATIC)
target_include_directories(Groak PRIVATE "include")

#FreeType beeing FreeType requiers to add include dir directly to it's own include dir
target_include_directories(Groak PUBLIC "Vendor/FreeType/include")

#link all of the libraries
target_link_libraries(Groak PUBLIC glad glfw)
#target_link_libraries(Groak PUBLIC "OpenGL")


add_subdirectory("Test_Application")
