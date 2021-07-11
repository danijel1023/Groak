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
