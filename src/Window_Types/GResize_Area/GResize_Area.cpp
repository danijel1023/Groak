#include "GResize_Area.h"
#include "GApplication.h"
#include "GWindow.h"

GResize_Area::GResize_Area(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y, GRes Ancor, int Min_Size_X, int Min_Size_Y, int Max_Size_X, int Max_Size_Y)
    : GResize_Area(Parent, { Window_X, Window_Y }, { Screen_X, Screen_Y }, Ancor, { Min_Size_X, Min_Size_Y }, { Max_Size_X, Max_Size_Y }) {}

GResize_Area::GResize_Area(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, GRes Ancor, GSize Min_Size, GSize Max_Size)
    : GBasic_Window(Parent, Window, Screen), m_Ancor(Ancor), m_Min_Size(Min_Size), m_Max_Size(Max_Size) {
    m_Callback_Ptr = &GResize_Area::Callback_Func;
}

GResize_Area::~GResize_Area() {}



bool operator&(GRes Left, GRes Right) {
    return static_cast<int>(Left) & static_cast<int>(Right);
}

bool operator|(GRes Left, GRes Right) {
    return static_cast<int>(Left) & static_cast<int>(Right);
}


int GResize_Area::Callback_Func(const GEvent& Event) {
    if (Event.Type == GEType::Mouse) {
        switch (Event.Mouse_Message) {
            case GEMouse_Message::Down:
            {
                if (Event.Mouse_Button == GEMouse_Button::LMB) {
                    m_Tracking = true;
                }

                break;
            }

            case GEMouse_Message::Up:
            {
                if (Event.Mouse_Button == GEMouse_Button::LMB) {
                    m_Tracking = false;
                }

                break;
            }

            case GEMouse_Message::Move:
            {
                if (m_Tracking) {
                    GEvent Move, Resize;
                    Move.Data_Ptr = m_Main_Window;
                    Move.Core_Message = GECore_Message::Move;
                    Move.WP = m_Main_Window->Get_Screen();

                    Resize.Data_Ptr = m_Main_Window;
                    Resize.Core_Message = GECore_Message::Resize;
                    Resize.WS = m_Main_Window->Get_Window();


                    const GSize& Mouse_Diff = (Event.MP - m_Prev_MP);
                    if (m_Ancor | GRes::Top)  Move.WP.Y -= Mouse_Diff.Y;
                    if (m_Ancor | GRes::Left) Move.WP.X += Mouse_Diff.X;

                    if (m_Ancor | GRes::Top) Resize.WS.Y += Mouse_Diff.Y;
                    if (m_Ancor | GRes::Bot) Resize.WS.Y -= Mouse_Diff.Y;
                    if (m_Ancor | GRes::Left)  Resize.WS.X -= Mouse_Diff.X;
                    if (m_Ancor | GRes::Right) Resize.WS.X += Mouse_Diff.X;

                    const GPos& Screen = m_Main_Window->Get_Screen();
                    const GSize& Window = m_Main_Window->Get_Window();
                    if (Resize.WS.X < m_Min_Size.X) {
                        Move.WP.X = Screen.X;

                        if (m_Ancor | GRes::Left)
                            Move.WP.X += Window.X - m_Min_Size.X;
                    }

                    if (Resize.WS.Y < m_Min_Size.Y) {
                        Move.WP.Y = Screen.Y;

                        if (m_Ancor | GRes::Top)
                            Move.WP.Y += Window.Y - m_Min_Size.Y;
                    }

                    if (Resize.WS.X < m_Min_Size.X) Resize.WS.X = m_Min_Size.X;
                    if (Resize.WS.Y < m_Min_Size.Y) Resize.WS.Y = m_Min_Size.Y;

                    
                    std::cout << "Mouse diff: " << Mouse_Diff << std::endl;
                    std::cout << "Resize: " << Resize.WS << std::endl;
                    std::cout << "Move: " << Move.WP << std::endl;

                    GEvent Clear;
                    Clear.Data_Ptr = m_Main_Window;
                    Clear.Core_Message = GECore_Message::Clear;

                    GApp()->Post_Event(Resize);
                    if (m_Ancor | GRes::Top || m_Ancor | GRes::Left)
                        GApp()->Post_Event(Move);
                    GApp()->Post_Event(Clear);
                }
                else m_Prev_MP = Event.MP;

                break;
            }
        }
    }

    return GBasic_Window::Callback_Func(Event);
}
