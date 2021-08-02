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

    void Draw_Text(const GText& Str, const GPos& Pos);  //Use default font
    void Draw_Text(const GText& Str, const GPos& Pos, GFont* Font);

    GAtlas& Get_Atlas(GFont* Font, unsigned int Code_Point);
    GAtlas& Get_Empty_Atlas();
    void Fill_Atlas(GFont* Font, GAtlas& Atlas);
    

    //Call to add a quad to render
    void Add_Quad(const GQuad& Quad);

    void Flush();   //Flush / render to the framebuffer
    void Render();  //Exactly the same as Flush() - just different name
    void Clear();


    void Set_Window_Space(int Screen_X, int Screen_Y, int Window_X, int Window_Y);
    void Set_Scale(int X, int Y);

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
};
