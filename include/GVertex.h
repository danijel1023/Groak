#pragma once
#include "GUnits.h"

struct GColor {
    int Red = 0, Green = 0, Blue = 0, Alpha = 255;
};

struct GVertex {
    int Type = 0;
    GVec2 Pos = { 0, 0 };
    GVec2 Centre = { 0, 0 };
    GColor Color = { 0, 0, 0, 255 };

    GVec2 Tex_Coords = { 0, 0 };
    int Tex_id = 0;

    float Rotation = 0.0f;
};
