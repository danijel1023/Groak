#version 330 core

layout(location = 0) in ivec2   cpu_Pos;
layout(location = 1) in ivec2   cpu_Centre;
layout(location = 2) in vec3    cpu_Color;
layout(location = 3) in vec2    cpu_Tex_Coord;
layout(location = 4) in int     cpu_Tex_id;
layout(location = 5) in float   cpu_Alpha;
layout(location = 6) in float   cpu_Rotation;

out vec3        v_Color;
out vec2        v_Tex_Coord;
flat out int    v_Tex_id;
out float       v_Alpha;

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


    v_Color = cpu_Color;
    v_Alpha = cpu_Alpha;

    v_Tex_id = cpu_Tex_id;
    v_Tex_Coord = cpu_Tex_Coord;
    if (v_Tex_id < -1.0) v_Tex_Coord.y = 1.0 - v_Tex_Coord.y;
}
