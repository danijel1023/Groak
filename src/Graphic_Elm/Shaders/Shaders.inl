const char* Vertex_Shader = R"(
#version 330 core

layout(location = 0) in int     cpu_Type;
layout(location = 1) in ivec2   cpu_Pos;
layout(location = 2) in ivec2   cpu_Centre;
layout(location = 3) in vec4    cpu_Color;
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

    v_Color = cpu_Color;

    v_Tex_Slot = cpu_Tex_Slot;
    v_Tex_Coord = cpu_Tex_Coord;
    v_Type = cpu_Type;

    //1 -> Atlas
    if (v_Type == 1) v_Tex_Coord.y = 1.0 - v_Tex_Coord.y;
}

)";



const char* Fragment_Shader = R"(
#version 330 core

in vec4         v_Color;
in vec2         v_Tex_Coord;
flat in int     v_Tex_Slot;
flat in int     v_Type;

uniform sampler2D u_Textures[16];

out vec4 Color;

vec4 Get_Texture(int Tex_Slot) {
    switch (Tex_Slot) {
    case  0: return texture(u_Textures[ 0], v_Tex_Coord);
    case  1: return texture(u_Textures[ 1], v_Tex_Coord);
    case  2: return texture(u_Textures[ 2], v_Tex_Coord);
    case  3: return texture(u_Textures[ 3], v_Tex_Coord);
    case  4: return texture(u_Textures[ 4], v_Tex_Coord);
    case  5: return texture(u_Textures[ 5], v_Tex_Coord);
    case  6: return texture(u_Textures[ 6], v_Tex_Coord);
    case  7: return texture(u_Textures[ 7], v_Tex_Coord);
    case  8: return texture(u_Textures[ 8], v_Tex_Coord);
    case  9: return texture(u_Textures[ 9], v_Tex_Coord);
    case 10: return texture(u_Textures[10], v_Tex_Coord);
    case 11: return texture(u_Textures[11], v_Tex_Coord);
    case 12: return texture(u_Textures[11], v_Tex_Coord);
    case 13: return texture(u_Textures[13], v_Tex_Coord);
    case 14: return texture(u_Textures[14], v_Tex_Coord);
    case 15: return texture(u_Textures[15], v_Tex_Coord);
    }
}


void main() {
    switch (v_Type) {
    case 0: Color = v_Color; break;
    case 1: Color = vec4(1.0, 1.0, 1.0, Get_Texture(v_Tex_Slot).r); break;
    case 2: Color = Get_Texture(v_Tex_Slot) * v_Color; break;
    }
}

)";
