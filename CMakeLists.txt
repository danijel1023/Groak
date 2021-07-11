cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 17)

project(Groak VERSION 1.0.0 DESCRIPTION "First stable release")


file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_library(Groak STATIC ${SRC_FILES})

#add_compile_definitions(GLEW_STATIC)
target_include_directories(Groak PUBLIC "include")
target_include_directories(Groak PUBLIC "Vendor/GLFW/include")

#FreeType beeing FreeType requiers to add include dir directly to it's own include dir
target_include_directories(Groak PUBLIC "Vendor/FreeType/include")

target_link_libraries(Groak PRIVATE "OpenGL32")
target_link_libraries(Groak PRIVATE "${CMAKE_SOURCE_DIR}/Vendor/GLFW/lib/libglfw3.a")
target_link_libraries(Groak PRIVATE "${CMAKE_SOURCE_DIR}/Vendor/GLEW/lib/libglew32.dll.a")
target_link_libraries(Groak PRIVATE "${CMAKE_SOURCE_DIR}/Vendor/FreeType/lib/libfreetyped.a")

add_subdirectory("Test_Application")
