#pragma once
#include "GVertex.h"

struct GColor {
    float Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 1.0f;
};

enum class GQuad_Type :int { Quad = 0, Atlas = 1, Char = 2 };

struct GTexture;
struct GQuad {
    GQuad();
    GQuad(float Window_X, float Window_Y, float Screen_X, float Screen_Y);
    GQuad(const GVec2& Size, const GVec2& Pos);

    //To remove a quad from rendering, instead of removing it from the m_Quad_List altogether,
    //you just need to set this flag to false and when iterating through the list, if the flag
    //is set to false, it won't be passed to renderer. This eliminates overhead on removing
    //elements from (in this case) vector and allows "random access" removing
    //- removing through the iterator and in this case, through the actual object itself
    bool m_Active = true;

    GQuad_Type m_Type = GQuad_Type::Quad;
    GVec2 m_Window = { 0, 0 };
    GVec2 m_Screen = { 0, 0 };
    GColor m_Color;
    float m_Rotation = 0.0f;
    unsigned int m_Texture = -1;
    GVec2 m_Tex_Coords[4] = {
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0},
    };

    void Repeat_Texture(const GTexture& Texture, unsigned int X_Repeat, unsigned int Y_Repeat);
    void Texture_Region(const GTexture& Texture, const GSize& Window, const GPos& Screen);
    void Texture_Region(const GTexture& Texture, unsigned int Window_X, unsigned int Window_Y, unsigned int Screen_X, unsigned int Screen_Y);

    void Insert_Vertices(GVertex* Buffer, int Texture_Slot) const;
};
