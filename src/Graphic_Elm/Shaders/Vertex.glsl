#version 330 core

layout(location = 0) in int     cpu_Type;
layout(location = 1) in vec2    cpu_Pos;
layout(location = 2) in vec2    cpu_Centre;
layout(location = 3) in ivec4   cpu_Color;
layout(location = 4) in vec2    cpu_Tex_Coord;
layout(location = 5) in int     cpu_Tex_Slot;
layout(location = 6) in float   cpu_Rotation;

out vec4        v_Color;
out vec2        v_Tex_Coord;
flat out int    v_Tex_Slot;
flat out int    v_Type;

uniform vec2 u_Scale;

void main() {
    vec2 Pos = vec2(cpu_Pos);
    vec2 Centre = vec2(cpu_Centre);
    float Radiant = radians(cpu_Rotation);

    //Rotate the vertex
    Pos -= Centre;
    Pos = vec2(Pos.x * vec2(cos(Radiant), sin(Radiant))) + vec2(Pos.y * vec2(-sin(Radiant), cos(Radiant)));
    Pos += Centre;

    //Normalize pixel coordinates to (-1,1) coordinatets
    vec2 Scale = u_Scale / 2;
    Pos = (Pos / Scale) - 1.0;

    gl_Position = vec4(Pos, 1.0, 1.0);

    v_Color = vec4(cpu_Color.r / 255, cpu_Color.g / 255, cpu_Color.b / 255, cpu_Color.a / 255);

    v_Tex_Slot = cpu_Tex_Slot;
    v_Tex_Coord = cpu_Tex_Coord;
    v_Type = cpu_Type;

    //1 -> Atlas
    //if (v_Type == 1) v_Tex_Coord.y = 1.0 - v_Tex_Coord.y;
}
