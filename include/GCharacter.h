#pragma once
#include "GQuad.h"
#include "GString.h"
#include "GCore.h"
#include "GBasic_Framebuffer.h"

#include <map>
#include <vector>

struct GCharacter {
    GCharacter(int Code_Point, GColor Color)
        :Code_Point(Code_Point), Color(Color) {}

    unsigned int Code_Point = 0;
    GColor Color;
};

using GText = std::vector<GCharacter>;

//GAChar_Data - Groak Atlas Char Data
struct GAChar_Data {
    GPos Pos;
    GSize Size;
    GIVec2 Bearing;
    unsigned int Advance = 0;
};

struct GAtlas {
    unsigned int Min = 0, Max = 0;    //if (Min <= Char && Char <= Max)
    GBasic_Framebuffer* FB = nullptr;

    std::map<unsigned int, GAChar_Data> Map;
};


struct GFont {
    GString File;
    FT_Face Face = nullptr;

    int Scale_Height = 0;
    int Height = 0;
    int Descender = 0;
    int Max_Advance_Width = 0;
    std::vector<GAtlas> Atlas_List;
};
