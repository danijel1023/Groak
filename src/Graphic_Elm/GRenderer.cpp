#include <string>
#include <iostream>
#include <map>
#include "Shaders/Shaders.inl"

#include "GCore.h"
#include "GRenderer.h"

#define __Quad_Count 256
unsigned int Create_Shader(const std::string& Vertex, const std::string& Fragment);


GRenderer::GRenderer() {
    m_Buffer = new GVertex[__Quad_Count * 4];

    m_Shader = Create_Shader(Vertex_Shader, Fragment_Shader);
    int Samplers[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    glUniform1iv(glGetUniformLocation(m_Shader, "u_Textures"), 16, Samplers);
    m_Scale = glGetUniformLocation(m_Shader, "u_Scale");

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Vertex buffer and stride
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, __Quad_Count * 4 * sizeof(GVertex), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribIPointer(0, 2, GL_INT, sizeof(GVertex), (void*)offsetof(GVertex, Pos));
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(1, 2, GL_INT, sizeof(GVertex), (void*)offsetof(GVertex, Centre));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GVertex), (void*)offsetof(GVertex, Color));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GVertex), (void*)offsetof(GVertex, Tex_Coords));
    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 1, GL_INT, sizeof(GVertex), (void*)offsetof(GVertex, Tex_id));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(GVertex), (void*)offsetof(GVertex, Alpha));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(GVertex), (void*)offsetof(GVertex, Rotation));

    // Index (element) buffer
    unsigned int Indices[__Quad_Count * 6];
    for (unsigned int i = 0; i < __Quad_Count; i++) {
        Indices[(i * 6) + 0] = (i * 4) + 0;
        Indices[(i * 6) + 1] = (i * 4) + 1;
        Indices[(i * 6) + 2] = (i * 4) + 2;
        Indices[(i * 6) + 3] = (i * 4) + 2;
        Indices[(i * 6) + 4] = (i * 4) + 3;
        Indices[(i * 6) + 5] = (i * 4) + 0;
    }

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GRenderer::~GRenderer() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);

    glDeleteVertexArrays(1, &m_VAO);

    delete[] m_Buffer;
}


unsigned int GRenderer::Get_Shader() {
    return m_Shader;
}



void GRenderer::Set_Scale(int X, int Y) {
    glUniform2f(m_Scale, (float)X, (float)Y);
}

void GRenderer::Set_Window(int Screen_X, int Screen_Y, int Window_X, int Window_Y) {
    glViewport(Screen_X, Screen_Y, Window_X, Window_Y);
    Set_Scale(Window_X, Window_Y);
}



void GRenderer::Add_Quad(GQuad& Quad) {
    //Switching textures - if the texture is set
    if (Quad.m_Texture != -1) {
        if (Textures_id.find(Quad.m_Texture) == Textures_id.end()) {
            //Flush the buffer if more than 16 textures are used
            if (Current_Tex_id == 16) Render();
            
            //Characters need a custom texture offset from regular textures
            if (Quad.m_Character) Textures_id[Quad.m_Texture] = -(int)(2 + Current_Tex_id);
            else                  Textures_id[Quad.m_Texture] = Current_Tex_id;

            glActiveTexture(GL_TEXTURE0 + Current_Tex_id);
            glBindTexture(GL_TEXTURE_2D, Quad.m_Texture);

            Current_Tex_id++;
        }
    }
    else Textures_id[Quad.m_Texture] = -1;  //I might reconsider this function - a bit of optimisation

    //Copy the vertices of quad to memory
    Quad.Insert_Vertices(&m_Buffer[Quad_i * 4], Textures_id[Quad.m_Texture]);
    Quad_i += 1;
}



void GRenderer::Flush() { Render(); }
void GRenderer::Render() {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, Quad_i * 4 * sizeof(GVertex), m_Buffer);
    glDrawElements(GL_TRIANGLES, (GLsizei)Quad_i * 6, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Quad_i = 0;
    Current_Tex_id = 0;
    Textures_id.clear();
}



static unsigned int Compile_Shader(unsigned int Type, const std::string& Src) {
    unsigned int id = glCreateShader(Type);
    const char* _src = Src.c_str();
    glShaderSource(id, 1, &_src, nullptr);
    glCompileShader(id);

    int Result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
    if (Result == GL_FALSE) {
        int Length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &Length);
        char* Message = (char*)_malloca(Length * sizeof(char));

        glGetShaderInfoLog(id, Length, &Length, Message);
        std::cout << "[Error::Compile_Shader] " << Message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int Create_Shader(const std::string& Vertex, const std::string& Fragment) {
    unsigned int Program = glCreateProgram();
    unsigned int vs = Compile_Shader(GL_VERTEX_SHADER, Vertex);
    unsigned int fs = Compile_Shader(GL_FRAGMENT_SHADER, Fragment);

    //Linking stage
    glAttachShader(Program, vs);
    glAttachShader(Program, fs);

    glLinkProgram(Program);

    int Result;
    glGetProgramiv(Program, GL_LINK_STATUS, &Result);
    if (Result == GL_FALSE) {
        int Length;
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &Length);
        char* Message = (char*)_malloca(Length * sizeof(char));

        glGetProgramInfoLog(Program, Length, &Length, Message);
        std::cout << "[Error::Link_Program] " << Message << std::endl;

        glDeleteProgram(Program);
        return 0;
    }


    glValidateProgram(Program);

    //A bit of cleaning
    glDeleteShader(vs);
    glDeleteShader(fs);

    glUseProgram(Program);
    return Program;
}
