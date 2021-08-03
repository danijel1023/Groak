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
    case 0: {
        if (v_Tex_Slot == -1)   Color = v_Color;
        else                    Color = Get_Texture(v_Tex_Slot);

        break;
    }

    case 1: {
        Color = vec4(1.0, 1.0, 1.0, Get_Texture(v_Tex_Slot).r);
        break;
    }

    case 2: {
        Color = Get_Texture(v_Tex_Slot) * v_Color;
        break;
    }

    default:
        Color = vec4(0.0);
        break;
    }
}
