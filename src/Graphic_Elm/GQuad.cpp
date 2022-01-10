#include "GQuad.h"
#include "GTexture.h"

GQuad::GQuad() {}

GQuad::GQuad(int Window_X, int Window_Y, int Screen_X, int Screen_Y)
    : GQuad({ static_cast<float>(Window_X), static_cast<float>(Window_Y) }, { static_cast<float>(Screen_X), static_cast<float>(Screen_Y) }) {}

GQuad::GQuad(float Window_X, float Window_Y, float Screen_X, float Screen_Y)
    : GQuad({ Window_X, Window_Y }, { Screen_X, Screen_Y }) {}

GQuad::GQuad(const GVec2& Size, const GVec2& Pos)
    : m_Window(Size), m_Screen(Pos) {
    Centre_Rot_Point();
}


void GQuad::Repeat_Texture(const GTexture& Texture, float X_Repeat, float Y_Repeat) {
    m_Texture = Texture.ID;

    m_Tex_Coords[1].X = X_Repeat;
    m_Tex_Coords[2] = { X_Repeat, Y_Repeat };
    m_Tex_Coords[3].Y = Y_Repeat;
}


void GQuad::Texture_Region(const GTexture& Texture, const GSize& Window, const GPos& Screen) {
    Texture_Region(Texture, static_cast<float>(Window.X), static_cast<float>(Window.Y), static_cast<float>(Screen.X), static_cast<float>(Screen.Y));
}

void GQuad::Texture_Region(const GTexture& Texture, float Window_X, float Window_Y, float Screen_X, float Screen_Y) {
    Window_X /= Texture.Size.X;
    Window_Y /= Texture.Size.Y;
    Screen_X /= Texture.Size.X;
    Screen_Y /= Texture.Size.Y;

    m_Tex_Coords[0] = { Screen_X,             Screen_Y };
    m_Tex_Coords[1] = { Screen_X + Window_X,  Screen_Y };
    m_Tex_Coords[2] = { Screen_X + Window_X,  Screen_Y + Window_Y };
    m_Tex_Coords[3] = { Screen_X,             Screen_Y + Window_Y };

    m_Texture = Texture.ID;
}

void GQuad::Centre_Rot_Point() {
    m_Rot_Point = { (m_Window.X / 2), (m_Window.Y / 2) };
}


void GQuad::Insert_Vertices(GVertex* Buffer, int Texture_Slot) const {
    int32_t Type = (int32_t)m_Type;

    if (Flip_Texture)   Type |= 0x0100;
    else                Type &= ~0x0100;

    GVec2 Rotation_Point = m_Rot_Point + m_Screen;

    Buffer[0] = GVertex{ Type, {m_Screen.X,              m_Screen.Y             }, Rotation_Point, m_Color, m_Tex_Coords[0], Texture_Slot, m_Rotation };
    Buffer[1] = GVertex{ Type, {m_Screen.X + m_Window.X, m_Screen.Y             }, Rotation_Point, m_Color, m_Tex_Coords[1], Texture_Slot, m_Rotation };
    Buffer[2] = GVertex{ Type, {m_Screen.X + m_Window.X, m_Screen.Y + m_Window.Y}, Rotation_Point, m_Color, m_Tex_Coords[2], Texture_Slot, m_Rotation };
    Buffer[3] = GVertex{ Type, {m_Screen.X,              m_Screen.Y + m_Window.Y}, Rotation_Point, m_Color, m_Tex_Coords[3], Texture_Slot, m_Rotation };
}
