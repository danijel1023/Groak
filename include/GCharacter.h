#pragma once
#include "GEvent.h"
#include "GQuad.h"

struct GCharacter {
    GCharacter(int Code_Point, GColor Color, GPos Pos)
        :m_Code_Point(Code_Point), m_Color(Color), m_Pos(Pos) {}

    int m_Code_Point = 0;
    GColor m_Color;
    GPos m_Pos;
};
