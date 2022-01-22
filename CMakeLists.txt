cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 17)

project(Groak VERSION 0.1.0)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

#Update Shaders file on cmake-proj configure
set(OUTPUT_FILE_LOC ${CMAKE_CURRENT_BINARY_DIR})
include("src/Graphic_Elm/Shaders/Update_Shaders.cmake")

#Update Shaders file on build
add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Shaders.inl"
    COMMAND ${CMAKE_COMMAND} -DOUTPUT_FILE_LOC=${CMAKE_CURRENT_BINARY_DIR} -P "${CMAKE_SOURCE_DIR}/src/Graphic_Elm/Shaders/Update_Shaders.cmake"
    DEPENDS "${CMAKE_SOURCE_DIR}/src/Graphic_Elm/Shaders/Vertex.glsl" "${CMAKE_SOURCE_DIR}/src/Graphic_Elm/Shaders/Fragment.glsl" "${CMAKE_SOURCE_DIR}/src/Graphic_Elm/Shaders/Shaders.inl.in"
)


#Create library
file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_library(Groak STATIC ${SRC_FILES} "${CMAKE_CURRENT_BINARY_DIR}/Shaders.inl")
target_compile_definitions(Groak PRIVATE GROAK_BUILD)
target_compile_definitions(Groak PRIVATE Shaders_File="${CMAKE_CURRENT_BINARY_DIR}/Shaders.inl")


#Build glad & glfw libraries
add_subdirectory("Vendor/glad")

option(GLFW_BUILD_DOCS OFF FORCE)
option(GLFW_BUILD_TESTS OFF FORCE)
option(GLFW_BUILD_EXAMPLES OFF FORCE)
add_subdirectory("Vendor/glfw3")
add_subdirectory("Vendor/freetype")
add_subdirectory("Vendor/stb")

option(MSDFGEN_BUILD_STANDALONE OFF FORCE)
option(MSDFGEN_INSTALL OFF FORCE)
set(FREETYPE_LIBRARY "${CMAKE_SOURCE_DIR}/Vendor/freetype")
set(FREETYPE_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/Vendor/freetype/include")
add_subdirectory("Vendor/msdfgen")


target_compile_definitions(Groak PRIVATE GLEW_STATIC)
target_include_directories(Groak PRIVATE "include")

#FreeType beeing FreeType requiers to add include dir directly to it's own include dir
target_include_directories(Groak PUBLIC "Vendor/freetype/include")

#Link all of the libraries
target_link_libraries(Groak PRIVATE glad glfw freetype stb msdfgen-ext)


add_subdirectory("Test_Application")
