#pragma once
#include "GEvent.h"
#include "GQuad.h"

struct GCharacter {
    GCharacter(int Code_Point, GColor Color)
        :m_Code_Point(Code_Point), m_Color(Color) {}

    int m_Code_Point = 0;
    GColor m_Color;
};
