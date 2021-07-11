#pragma once

struct vec2 { float X = 0, Y = 0; };
struct vec3 { float X = 0, Y = 0, Z = 0; };
struct vec4 { float X = 0, Y = 0, Z = 0, W = 0; };

struct ivec2 { int X = 0, Y = 0; };
struct ivec3 { int X = 0, Y = 0, Z = 0; };
struct ivec4 { int X = 0, Y = 0, Z = 0, W = 0; };

struct uivec2 { unsigned int X = 0, Y = 0; };
struct uivec3 { unsigned int X = 0, Y = 0, Z = 0; };
struct uivec4 { unsigned int X = 0, Y = 0, Z = 0, W = 0; };

struct GVertex {
    ivec2 Pos = { 0, 0 };
    ivec2 Centre = { 0, 0 };
    vec4 Color = { 0, 0, 0, 1 };

    vec2 Tex_Coords = { 0, 0 };
    int Tex_id = 0;

    float Rotation = 0.0f;
};
