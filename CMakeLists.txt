cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 17)

project(Groak VERSION 1.0.0 DESCRIPTION "First stable relese")


file(GLOB_RECURSE SRC_FILES src/*.cpp)
add_library(Groak STATIC ${SRC_FILES})

add_compile_definitions(GLEW_STATIC)
target_include_directories(Groak PUBLIC "include")
target_include_directories(Groak PUBLIC "Vendor/glfw3/include")

#FreeType beeing FreeType requiers to add include dir directly to it's own include dir
target_include_directories(Groak PUBLIC "Vendor/FreeType-2.10.4/include")

target_link_libraries(Groak PRIVATE "OpenGL32")
target_link_libraries(Groak PRIVATE "${CMAKE_SOURCE_DIR}/Vendor/glfw3/lib/win64/glfw3.lib")
target_link_libraries(Groak PRIVATE "${CMAKE_SOURCE_DIR}/Vendor/glew-2.1.0/lib/win64/glew32s.lib")
target_link_libraries(Groak PRIVATE "${CMAKE_SOURCE_DIR}/Vendor/FreeType-2.10.4/lib/win64/freetype.lib")

add_subdirectory("Test_Application")
