#pragma once
#include "GUnits.h"

struct GVertex {
    int Type = 0;
    GVec2 Pos = { 0, 0 };
    GVec2 Centre = { 0, 0 };
    GVec4 Color = { 0, 0, 0, 1 };

    GVec2 Tex_Coords = { 0, 0 };
    int Tex_id = 0;

    float Rotation = 0.0f;
};
