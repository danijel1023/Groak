#include "GQuad.h"
#include "GTexture.h"

GQuad::GQuad() {}

GQuad::GQuad(const GVec2& Size, const GVec2& Pos)
    : m_Window(Size), m_Screen(Pos) {}
GQuad::GQuad(float Window_X, float Window_Y, float Screen_X, float Screen_Y)
    : m_Window({ Window_X, Window_Y }), m_Screen({ Screen_X, Screen_Y }) {}


void GQuad::Repeat_Texture(const GTexture& Texture, unsigned int X_Repeat, unsigned int Y_Repeat) {
    m_Texture = Texture.ID;

    m_Tex_Coords[1].X = (float)X_Repeat;
    m_Tex_Coords[2] = { (float)X_Repeat, (float)Y_Repeat };
    m_Tex_Coords[3].Y = (float)Y_Repeat;
}


void GQuad::Texture_Region(const GTexture& Texture, const GSize& Window, const GPos& Screen) {
    Texture_Region(Texture, Window.X, Window.Y, Screen.X, Screen.Y);
}

void GQuad::Texture_Region(const GTexture& Texture, unsigned int Window_X, unsigned int Window_Y, unsigned int Screen_X, unsigned int Screen_Y) {
    float Window_Xf = (float)Window_X, Window_Yf = (float)Window_Y, Screen_Xf = (float)Screen_X, Screen_Yf = (float)Screen_Y;
    Window_Xf /= Texture.Size.X;
    Window_Yf /= Texture.Size.Y;
    Screen_Xf /= Texture.Size.X;
    Screen_Yf /= Texture.Size.Y;

    m_Tex_Coords[0] = { Screen_Xf,              Screen_Yf };
    m_Tex_Coords[1] = { Screen_Xf + Window_Xf,  Screen_Yf };
    m_Tex_Coords[2] = { Screen_Xf + Window_Xf,  Screen_Yf + Window_Yf };
    m_Tex_Coords[3] = { Screen_Xf,              Screen_Yf + Window_Yf };

    m_Texture = Texture.ID;
}

void GQuad::Insert_Vertices(GVertex* Buffer, int Texture_Slot) const {
    int32_t Data = (int32_t)m_Type;

    if (Flip_Texture)   Data |= 0x0100;
    else                Data &= ~0x0100;
    
    Buffer[0] = GVertex{ Data, m_Screen.X,              m_Screen.Y,              m_Screen.X + (m_Window.X / 2), m_Screen.Y + (m_Window.Y / 2), m_Color.Red, m_Color.Green, m_Color.Blue, m_Color.Alpha, m_Tex_Coords[0], Texture_Slot, m_Rotation };
    Buffer[1] = GVertex{ Data, m_Screen.X + m_Window.X, m_Screen.Y,              m_Screen.X + (m_Window.X / 2), m_Screen.Y + (m_Window.Y / 2), m_Color.Red, m_Color.Green, m_Color.Blue, m_Color.Alpha, m_Tex_Coords[1], Texture_Slot, m_Rotation };
    Buffer[2] = GVertex{ Data, m_Screen.X + m_Window.X, m_Screen.Y + m_Window.Y, m_Screen.X + (m_Window.X / 2), m_Screen.Y + (m_Window.Y / 2), m_Color.Red, m_Color.Green, m_Color.Blue, m_Color.Alpha, m_Tex_Coords[2], Texture_Slot, m_Rotation };
    Buffer[3] = GVertex{ Data, m_Screen.X,              m_Screen.Y + m_Window.Y, m_Screen.X + (m_Window.X / 2), m_Screen.Y + (m_Window.Y / 2), m_Color.Red, m_Color.Green, m_Color.Blue, m_Color.Alpha, m_Tex_Coords[3], Texture_Slot, m_Rotation };
}
