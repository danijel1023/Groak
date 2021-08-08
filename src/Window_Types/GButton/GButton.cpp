#include "GButton.h"
#include "GWindow.h"

GButton::GButton(GBasic_Window* Parent, const GSize& Window, const GPos& Screen)
    : GBasic_Window(Parent, Window, Screen) {
    m_Callback_Ptr = &GButton::Callback_Func;
}

GButton::~GButton() {}

GQuad& GButton::Get_Color_Quad() {
    return Get_Quad(m_Color_Quad);
}

GQuad& GButton::Get_Texture_Quad() {
    return Get_Quad(m_Texture_Quad);
}


int GButton::Callback_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Run:
                {
                    GQuad Color(m_Window, { 0, 0 });
                    Color.m_Color = m_Leave;
                    m_Color_Quad = Add_Quad(Color);

                    GQuad Texture(m_Window, { 0, 0 });
                    Texture.m_Texture = m_Texture.ID;
                    Texture.m_Active = m_Texture.ID;
                    m_Texture_Quad = Add_Quad(Texture);
                    break;
                }

                case GEWind_Message::Lose_Focus:
                {
                    Get_Quad(m_Color_Quad).m_Color = m_Leave;
                    m_Pressed = false;
                    Render();
                }
            }

            break;
        }

        case GEType::Mouse:
        {
            switch (Event.Mouse_Message) {
                case GEMouse_Message::Down:
                {
                    if (Event.Mouse_Button == GEMouse_Button::LMB) {
                        m_Pressed = true;

                        Get_Quad(m_Color_Quad).m_Color = m_Press;
                        Render();
                    }

                    break;
                }

                case GEMouse_Message::Up:
                {
                    if (Event.Mouse_Button == GEMouse_Button::LMB) {
                        m_Pressed = false;


                        if (0 <= Event.MP.X && Event.MP.X <= m_Window.X && 0 <= Event.MP.Y && Event.MP.Y <= m_Window.Y) {
                            m_Main_Window->Post_Event(m_On_Press_Event);
                            Get_Quad(m_Color_Quad).m_Color = m_Hover;
                        }

                        else {
                            Get_Quad(m_Color_Quad).m_Color = m_Leave;
                        }

                        Render();
                    }

                    break;
                }

                case GEMouse_Message::Move:
                {
                    if (0 <= Event.MP.X && Event.MP.X <= m_Window.X && 0 <= Event.MP.Y && Event.MP.Y <= m_Window.Y) {
                        if (m_Pressed) {
                            Get_Quad(m_Color_Quad).m_Color = m_Press;
                        }

                        else {
                            Get_Quad(m_Color_Quad).m_Color = m_Hover;
                        }
                    }

                    else {
                        Get_Quad(m_Color_Quad).m_Color = m_Leave;
                    }

                    Render();
                    break;
                }

                case GEMouse_Message::Leave:
                {
                    Get_Quad(m_Color_Quad).m_Color = m_Leave;
                    m_Pressed = false;
                    Render();

                    break;
                }

                break;
            }

            return 1;
        }
    }

    return GBasic_Window::Callback_Func(Event);
}
