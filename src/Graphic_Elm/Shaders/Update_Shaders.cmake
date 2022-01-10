file(READ "${CMAKE_CURRENT_LIST_DIR}/Vertex.glsl" Vertex_Shader_File)
file(READ "${CMAKE_CURRENT_LIST_DIR}/Fragment.glsl" Fragment_Shader_File)

configure_file("${CMAKE_CURRENT_LIST_DIR}/Shaders.inl.in" "${OUTPUT_FILE_LOC}/Shaders.inl")
