#include "GTitle_Bar.h"
#include "GWindow.h"
#include "GApplication.h"
#include "GLog_Manager.h"

#include "res.inl"


GTitle_Bar::GTitle_Bar(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y)
    :GTitle_Bar(Parent, { Window_X, Window_Y }, { Screen_X, Screen_Y }) {}

GTitle_Bar::GTitle_Bar(GBasic_Window* Parent, const GSize& Window, const GPos& Screen)
    : GBasic_Window(Parent, Window, Screen) {
    m_Callback_Ptr = &GTitle_Bar::Callback_Func;
}

GTitle_Bar::~GTitle_Bar() {}


int GTitle_Bar::Callback_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Run:
                {
                    GEvent On_Click;
                    On_Click.Type = GEType::Window;
                    On_Click.Data_Ptr = m_Main_Window;

                    On_Click.Wind_Message = GEWind_Message::Close;
                    m_Close = new GButton(this, { 46, 32 }, { m_Window.X - 1 * 46, 0 });
                    m_Close->m_Texture = m_Main_Window->Load_Texture_From_Memory(PNG_Close_Data, sizeof(PNG_Close_Data));
                    m_Close->m_On_Press_Event = On_Click;
                    m_Close->m_Leave = { 0, 0, 0, 0 };
                    m_Close->m_Hover = { (70 * 255 / 100), (0 * 255 / 100), (0 * 255 / 100), (100 * 255 / 100) };
                    m_Close->m_Press = { (60 * 255 / 100), (0 * 255 / 100), (0 * 255 / 100), (100 * 255 / 100) };
                    

                    On_Click.Wind_Message = GEWind_Message::Should_Iconify;
                    m_Iconify = new GButton(this, { 46, 32 }, { m_Window.X - 3 * 46, 0 });
                    m_Iconify->m_Texture = m_Main_Window->Load_Texture_From_Memory(PNG_Iconify_Data, sizeof(PNG_Iconify_Data));
                    m_Iconify->m_On_Press_Event = On_Click;
                    m_Iconify->m_Leave = { 0, 0, 0, 0 };
                    m_Iconify->m_Hover = { (50 * 255 / 100), (50 * 255 / 100), (50 * 255 / 100), (100 * 255 / 100) };
                    m_Iconify->m_Press = { (60 * 255 / 100), (60 * 255 / 100), (60 * 255 / 100), (100 * 255 / 100) };


                    m_Maximise_Tex = m_Main_Window->Load_Texture_From_Memory(PNG_Maximise_Data, sizeof(PNG_Maximise_Data));
                    m_Restore_Tex = m_Main_Window->Load_Texture_From_Memory(PNG_Restore_Data, sizeof(PNG_Restore_Data));
                    On_Click.Wind_Message = GEWind_Message::Should_Maximise;
                    m_Maximise = new GButton(this, { 46, 32 }, { m_Window.X - 2 * 46, 0 });
                    m_Maximise->m_Texture = m_Maximise_Tex;
                    m_Maximise->m_On_Press_Event = On_Click;
                    m_Maximise->m_Leave = { 0, 0, 0, 0 };
                    m_Maximise->m_Hover = { (50 * 255 / 100), (50 * 255 / 100), (50 * 255 / 100), (100 * 255 / 100) };
                    m_Maximise->m_Press = { (60 * 255 / 100), (60 * 255 / 100), (60 * 255 / 100), (100 * 255 / 100) };


                    GQuad Quad(m_Window.Convert_Type<float>(), { 0, 0 });
                    Quad.m_Color = m_Bkg_Color;
                    m_Bkg_Quad = Add_Quad(Quad);

                    if (!m_Iconified && !m_Maximised) {
                        m_Prev_Win_Size = m_Main_Window->Get_Window();
                    }
                    break;
                }

                case GEWind_Message::Resize:
                {
                    GBasic_Window::Callback_Func(Event);
                    Get_Quad(m_Bkg_Quad).m_Window = m_Window.Convert_Type<float>();

                    GEvent Event;
                    Event.Type = GEType::Window;
                    Event.Wind_Message = GEWind_Message::Move;

                    Event.WP = { m_Window.X - 1 * 46, 0 };
                    Event.Data_Ptr = m_Close;
                    m_Main_Window->Post_Event(Event);

                    Event.WP = { m_Window.X - 3 * 46, 0 };
                    Event.Data_Ptr = m_Iconify;
                    m_Main_Window->Post_Event(Event);

                    Event.WP = { m_Window.X - 2 * 46, 0 };
                    Event.Data_Ptr = m_Maximise;
                    m_Main_Window->Post_Event(Event);

                    if (!m_Iconified && !m_Maximised) {
                        m_Prev_Win_Size = m_Main_Window->Get_Window();
                    }

                    break;
                }

                case GEWind_Message::Iconify:
                {
                    m_Iconified = true;
                    break;
                }

                case GEWind_Message::Maximise:
                {
                    GEvent On_Click;
                    On_Click.Type = GEType::Window;
                    On_Click.Data_Ptr = m_Main_Window;
                    On_Click.Wind_Message = GEWind_Message::Should_Restore;
                    m_Maximise->m_On_Press_Event = On_Click;
                    m_Maximise->Get_Texture_Quad().m_Texture = m_Restore_Tex.ID;

                    m_Maximised = true;

                    break;
                }

                case GEWind_Message::Restore:
                {
                    if (!m_Iconified) {
                        GEvent On_Click;
                        On_Click.Type = GEType::Window;
                        On_Click.Data_Ptr = m_Main_Window;
                        On_Click.Wind_Message = GEWind_Message::Should_Maximise;
                        m_Maximise->m_On_Press_Event = On_Click;
                        m_Maximise->Get_Texture_Quad().m_Texture = m_Maximise_Tex.ID;
                    }

                    m_Iconified = false;
                    m_Maximised = false;
                    break;
                }
            }

            break;
        }

        case GEType::Mouse:
        {
            if (Event.Mouse_Message == GEMouse_Message::Move) {
                if (m_Track_Mouse) {
                    if (m_Maximised) {
                        GEvent Should_Restore;
                        Should_Restore.Type = GEType::Window;
                        Should_Restore.Data_Ptr = m_Main_Window;
                        Should_Restore.Wind_Message = GEWind_Message::Should_Restore;
                        m_Main_Window->Post_Event(Should_Restore);

                        m_Prev_MP.X = (m_Prev_MP.X * (m_Prev_Win_Size.X - 3*46)) / (m_Main_Window->Get_Window().X - 3*46);
                    }
                    
                    GEvent Move;
                    Move.Data_Ptr = m_Main_Window;
                    Move.Core_Message = GECore_Message::Move;

                    Move.WP = m_Main_Window->Get_Screen();
                    Move.WP.X += Event.MP.X - m_Prev_MP.X;
                    Move.WP.Y -= Event.MP.Y - m_Prev_MP.Y;
                    GApp()->Send_Event(Move);
                }
                else m_Prev_MP = Event.MP;
            }

            else if (Event.Mouse_Button == GEMouse_Button::LMB) {
                if (Event.Mouse_Message == GEMouse_Message::Down) {
                    m_Track_Mouse = true;
                }

                else if (Event.Mouse_Message == GEMouse_Message::Up) {
                    m_Track_Mouse = false;
                }
            }

            return 1;
        }
    }

    return GBasic_Window::Callback_Func(Event);
}
