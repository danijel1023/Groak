#pragma once
#include <map>
#include "GQuad.h"


#define GL_CALL(x)\
while (glGetError() != GL_NO_ERROR);\
x;\
std::cout << #x << std::endl;\
while (GLenum Error = glGetError()) { std::cout << "[OpenGL Error]: " << Error << std::endl; __debugbreak(); } 0


class GRenderer {
public:
    GRenderer();
    ~GRenderer();

    //Call to add a quad to render
    void Add_Quad(GQuad& Quad);

    
    void Flush();   //Flush / render to the framebuffer
    void Render();  //Exactly the same as Flush() - just different name

    void Set_Scale(int X, int Y);
    void Set_Window(int Screen_X, int Screen_Y, int Window_X, int Window_Y);

    unsigned int Get_Shader();

private:
    unsigned int m_VAO = 0, m_EBO = 0, m_VBO = 0;
    unsigned int m_Shader = 0;
    GVertex* m_Buffer = nullptr;

    int m_Scale = 0;

    size_t Quad_i = 0;
    std::map<unsigned int, int> Textures_id;
    unsigned int Current_Tex_id = 0;
};
