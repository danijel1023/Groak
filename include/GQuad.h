#pragma once
#include "GVertex.h"

//TODO: Add circle support
enum class GQuad_Type :uint32_t { Quad = 0, Atlas = 1, Char = 2, Circle = 3};

struct GTexture;
struct GQuad {
    GQuad();
    GQuad(float Window_X, float Window_Y, float Screen_X, float Screen_Y);
    GQuad(const GVec2& Size, const GVec2& Pos);

    //removing warnings
    GQuad(int Window_X, int Window_Y, int Screen_X, int Screen_Y);


    bool m_Active = true;

    GQuad_Type m_Type = GQuad_Type::Quad;
    bool Flip_Texture = false;

    GVec2 m_Window = { 0, 0 };
    GVec2 m_Screen = { 0, 0 };
    GVec2 m_Rot_Point = { 0, 0 };
    GColor m_Color;
    float m_Rotation = 0.0f;
    unsigned int m_Texture = -1;
    GVec2 m_Tex_Coords[4] = {
        {0.0, 0.0},
        {1.0, 0.0},
        {1.0, 1.0},
        {0.0, 1.0},
    };

    void Repeat_Texture(const GTexture& Texture, float X_Repeat, float Y_Repeat);
    void Texture_Region(const GTexture& Texture, const GSize& Window, const GPos& Screen);
    void Texture_Region(const GTexture& Texture, float Window_X, float Window_Y, float Screen_X, float Screen_Y);
    void Centre_Rot_Point();

    void Insert_Vertices(GVertex* Buffer, int Texture_Slot) const;
};
