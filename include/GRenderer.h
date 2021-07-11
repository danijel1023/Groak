#pragma once
#include <map>
#include "GQuad.h"
#include "GCharacter.h"

class GBasic_Framebuffer;
class GWindow;

class GRenderer {
public:
    GRenderer(GWindow* Main_Wind);
    ~GRenderer();

    //Call to add a quad to render
    void Add_Quad(GQuad& Quad);
    void Add_Char(GCharacter& CH);

    
    void Flush();   //Flush / render to the framebuffer
    void Render();  //Exactly the same as Flush() - just different name

    void Set_Scale(int X, int Y);
    void Set_Window_Screen(int Screen_X, int Screen_Y, int Window_X, int Window_Y);

    unsigned int Get_Shader();

private:
    GWindow* m_Main_Wind;
    unsigned int m_VAO = 0, m_EBO = 0, m_VBO = 0;
    unsigned int m_Shader = 0;
    GVertex* m_Buffer = nullptr;

    int m_Scale = 0;

    size_t Quad_i = 0;
    std::map<unsigned int, int> Textures_id;
    unsigned int Current_Tex_id = 0;

    std::map<int, GBasic_Framebuffer*> m_Sprite_List;
};
