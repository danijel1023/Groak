#pragma once
#include "GVertex.h"

struct GTexture;
struct GQuad {
    GQuad();
    GQuad(int Window_X, int Window_Y, int Screen_X, int Screen_Y);

    //To remove a quad from rendering, instead of removing it from the m_Quad_List altogether,
    //you just need to set this flag to false and when iterating through the list, if the flag
    //is set to false, it won't be passed to renderer. This eliminates overhead on removing
    //elements from (in this case) vector and allows "random access" removing
    //- removing through the iterator and in this case, through the actual object itself
    bool m_Active = true;

    bool m_Character = false;
    ivec2 m_Window = { 0, 0 };
    ivec2 m_Screen = { 0, 0 };
    float m_Red = 0.0f, m_Green = 0.0f, m_Blue = 0.0f, m_Alpha = 1.0f;
    float m_Rotation = 0.0f;
    unsigned int m_Texture = -1;
    vec2 m_Tex_Coords[4] = {
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0},
    };

    void Repeat_Texture(const GTexture& Texture, unsigned int X_Repeat, unsigned int Y_Repeat);
    void Texture_Region(const GTexture& Texture, unsigned int Window_X, unsigned int Window_Y, unsigned int Screen_X, unsigned int Screen_Y);

    void GQuad::Insert_Vertices(GVertex* Buffer, int Texture_Slot);
};
