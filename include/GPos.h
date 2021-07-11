#pragma once

struct GPos {
    int X = 0, Y = 0;

    GPos operator+(GPos Right) { return { (X + Right.X), (Y + Right.Y) }; }
    GPos operator-(GPos Right) { return { (X - Right.X), (Y - Right.Y) }; }

    GPos& operator+=(GPos Right) { X += Right.X; Y += Right.Y; return *this; }
    GPos& operator-=(GPos Right) { X -= Right.X; Y -= Right.Y; return *this; }
};
using GSize = GPos;
