const char* Vertex_Shader = R"(
#version 330 core

layout(location = 0) in ivec2   cpu_Pos;
layout(location = 1) in ivec2   cpu_Centre;
layout(location = 2) in vec4    cpu_Color;
layout(location = 3) in vec2    cpu_Tex_Coord;
layout(location = 4) in int     cpu_Tex_Slot;
layout(location = 5) in float   cpu_Rotation;

out vec4        v_Color;
out vec2        v_Tex_Coord;
flat out int    v_Tex_Slot;

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

    if (v_Tex_Slot < -1.0) v_Tex_Coord.y = 1.0 - v_Tex_Coord.y;
}

)";



const char* Fragment_Shader = R"(
#version 330 core

in vec4         v_Color;
in vec2         v_Tex_Coord;
flat in int     v_Tex_Slot;

uniform sampler2D u_Textures[16];

out vec4 Color;

vec4 Get_Texture(int Tex_Slot) {
    vec4 r_Color;

    switch (Tex_Slot) {
    case  0: r_Color = texture(u_Textures[ 0], v_Tex_Coord); break;
    case  1: r_Color = texture(u_Textures[ 1], v_Tex_Coord); break;
    case  2: r_Color = texture(u_Textures[ 2], v_Tex_Coord); break;
    case  3: r_Color = texture(u_Textures[ 3], v_Tex_Coord); break;
    case  4: r_Color = texture(u_Textures[ 4], v_Tex_Coord); break;
    case  5: r_Color = texture(u_Textures[ 5], v_Tex_Coord); break;
    case  6: r_Color = texture(u_Textures[ 6], v_Tex_Coord); break;
    case  7: r_Color = texture(u_Textures[ 7], v_Tex_Coord); break;
    case  8: r_Color = texture(u_Textures[ 8], v_Tex_Coord); break;
    case  9: r_Color = texture(u_Textures[ 9], v_Tex_Coord); break;
    case 10: r_Color = texture(u_Textures[10], v_Tex_Coord); break;
    case 11: r_Color = texture(u_Textures[11], v_Tex_Coord); break;
    case 12: r_Color = texture(u_Textures[11], v_Tex_Coord); break;
    case 13: r_Color = texture(u_Textures[13], v_Tex_Coord); break;
    case 14: r_Color = texture(u_Textures[14], v_Tex_Coord); break;
    case 15: r_Color = texture(u_Textures[15], v_Tex_Coord); break;
    }

    return r_Color;
}


void main() {
    //Quads
    if (v_Tex_Slot == -1) {
        Color = v_Color;
        //Color = vec4(1.0, 1.0, 1.0, 1.0);
    }

    //Textures
    else {
        if (v_Tex_Slot > -1) {
            Color = Get_Texture(v_Tex_Slot);
        }

        else {
            Color = vec4(1.0, 1.0, 1.0, Get_Texture(-(v_Tex_Slot + 2)).r) * v_Color;
        }
    }
}

)";
