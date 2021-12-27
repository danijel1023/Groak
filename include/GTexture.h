#pragma once
#include "GUnits.h"

struct GTexture {
    GIVec2 Size;
    int Channels = 0;
    unsigned int ID = 0;

    uint8_t* Pixels = nullptr;
};
