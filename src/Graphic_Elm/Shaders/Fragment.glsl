#version 330 core

in vec3         v_Color;
in vec2         v_Tex_Coord;
flat in int     v_Tex_id;
in float        v_Alpha;

uniform sampler2D u_Textures[16];

out vec4 Color;

void main() {
    //Quads
    if (v_Tex_id == -1) {
        Color = vec4(v_Color, 1.0);
        //Color = vec4(1.0, 1.0, 1.0, 1.0);
    }

    //Textures
    else {
        if (v_Tex_id > -1) {
            switch (v_Tex_id) {
            case  0: Color = texture(u_Textures[ 0], v_Tex_Coord); break;
            case  1: Color = texture(u_Textures[ 1], v_Tex_Coord); break;
            case  2: Color = texture(u_Textures[ 2], v_Tex_Coord); break;
            case  3: Color = texture(u_Textures[ 3], v_Tex_Coord); break;
            case  4: Color = texture(u_Textures[ 4], v_Tex_Coord); break;
            case  5: Color = texture(u_Textures[ 5], v_Tex_Coord); break;
            case  6: Color = texture(u_Textures[ 6], v_Tex_Coord); break;
            case  7: Color = texture(u_Textures[ 7], v_Tex_Coord); break;
            case  8: Color = texture(u_Textures[ 8], v_Tex_Coord); break;
            case  9: Color = texture(u_Textures[ 9], v_Tex_Coord); break;
            case 10: Color = texture(u_Textures[10], v_Tex_Coord); break;
            case 11: Color = texture(u_Textures[11], v_Tex_Coord); break;
            case 12: Color = texture(u_Textures[11], v_Tex_Coord); break;
            case 13: Color = texture(u_Textures[13], v_Tex_Coord); break;
            case 14: Color = texture(u_Textures[14], v_Tex_Coord); break;
            case 15: Color = texture(u_Textures[15], v_Tex_Coord); break;
            }
        }

        else {
            switch (v_Tex_id) {
            case  -2: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[ 0], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case  -3: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[ 1], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case  -4: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[ 2], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case  -5: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[ 3], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case  -6: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[ 4], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case  -7: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[ 5], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case  -8: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[ 6], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case  -9: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[ 7], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case -10: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[ 8], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case -11: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[ 9], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case -12: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[10], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case -13: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[11], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case -14: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[11], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case -15: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[13], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case -16: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[14], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            case -17: Color = vec4(1.0, 1.0, 1.0, texture(u_Textures[15], v_Tex_Coord).r) * vec4(v_Color, 1.0); break;
            }
        }
    }

    Color = vec4(Color.rgb, v_Alpha);
}
