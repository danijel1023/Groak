#include "GResize_Area.h"
#include "GApplication.h"
#include "GWindow.h"

GResize_Area::GResize_Area(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y, GResize_Ancor Ancor, int Min_Size_X, int Min_Size_Y, int Max_Size_X, int Max_Size_Y, bool Overlay)
    : GResize_Area(Parent, { Window_X, Window_Y }, { Screen_X, Screen_Y }, Ancor, { Min_Size_X, Min_Size_Y }, { Max_Size_X, Max_Size_Y }, Overlay) {}

GResize_Area::GResize_Area(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, GResize_Ancor Ancor, GSize Min_Size, GSize Max_Size, bool Overlay)
    : GBasic_Window(Parent, Window, Screen, Overlay), m_Ancor(Ancor), m_Min_Size(Min_Size), m_Max_Size(Max_Size) {
    m_Callback_Ptr = &GResize_Area::Callback_Func;
}

GResize_Area::~GResize_Area() {}



static bool operator&(const GResize_Ancor& Left, const GResize_Ancor& Right) {
    return static_cast<int>(Left) & static_cast<int>(Right);
}


int GResize_Area::Callback_Func(const GEvent& Event) {
    if (Event.Type == GEType::Mouse) {
        switch (Event.Mouse_Message) {
            case GEMouse_Message::Enter:
            {
                GEvent Event;
                Event.Core_Message = GECore_Message::Set_Cursor;
                Event.Data_Ptr = m_Main_Window;


                if (m_Ancor == GResize_Ancor::Top || m_Ancor == GResize_Ancor::Bot)
                    Event.Cursor_Type = GCursor_Type::Resize_NS;

                else if (m_Ancor == GResize_Ancor::Left || m_Ancor == GResize_Ancor::Right)
                    Event.Cursor_Type = GCursor_Type::Resize_EW;

                else if (m_Ancor == GResize_Ancor::Top_Left || m_Ancor == GResize_Ancor::Bot_Right)
                    Event.Cursor_Type = GCursor_Type::Resize_NWSE;

                else if (m_Ancor == GResize_Ancor::Bot_Left || m_Ancor == GResize_Ancor::Top_Right)
                    Event.Cursor_Type = GCursor_Type::Resize_NESW;


                GApp()->Post_Event(Event);
                break;
            }

            case GEMouse_Message::Leave:
            {
                GEvent Event;
                Event.Core_Message = GECore_Message::Set_Cursor;
                Event.Data_Ptr = m_Main_Window;
                Event.Cursor_Type = GCursor_Type::Default;
                GApp()->Post_Event(Event);
                break;
            }

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
                    const GPos& Screen = m_Main_Window->Get_Screen();
                    const GSize& Window = m_Main_Window->Get_Window();

                    Move.Data_Ptr = m_Main_Window;
                    Move.Core_Message = GECore_Message::Move;
                    Move.WP = Screen;

                    Resize.Data_Ptr = m_Main_Window;
                    Resize.Core_Message = GECore_Message::Resize;
                    Resize.WS = Window;


                    const GSize& Mouse_Diff = (Event.MP - m_Prev_MP);
                    if (m_Ancor & GResize_Ancor::Top)  Move.WP.Y -= Mouse_Diff.Y;
                    if (m_Ancor & GResize_Ancor::Left) Move.WP.X += Mouse_Diff.X;

                    if (m_Ancor & GResize_Ancor::Top) Resize.WS.Y += Mouse_Diff.Y;
                    if (m_Ancor & GResize_Ancor::Bot) Resize.WS.Y -= Mouse_Diff.Y;
                    if (m_Ancor & GResize_Ancor::Left)  Resize.WS.X -= Mouse_Diff.X;
                    if (m_Ancor & GResize_Ancor::Right) Resize.WS.X += Mouse_Diff.X;

                    
                    if (Resize.WS.X < m_Min_Size.X) {
                        Resize.WS.X = m_Min_Size.X;
                        Move.WP.X = Screen.X;

                        if (m_Ancor & GResize_Ancor::Left)
                            Move.WP.X += Window.X - m_Min_Size.X;
                    }

                    if (Resize.WS.Y < m_Min_Size.Y) {
                        Resize.WS.Y = m_Min_Size.Y;
                        Move.WP.Y = Screen.Y;

                        if (m_Ancor & GResize_Ancor::Top)
                            Move.WP.Y += Window.Y - m_Min_Size.Y;
                    }


                    if (Resize.WS.X > m_Max_Size.X) {
                        Resize.WS.X = m_Max_Size.X;
                        Move.WP.X = Screen.X;
                    
                        if (m_Ancor & GResize_Ancor::Left)
                            Move.WP.X += Window.X - m_Max_Size.X;
                    }
                    
                    if (Resize.WS.Y > m_Max_Size.Y) {
                        Resize.WS.Y = m_Max_Size.Y;
                        Move.WP.Y = Screen.Y;
                    
                        if (m_Ancor & GResize_Ancor::Top)
                            Move.WP.Y += Window.Y - m_Max_Size.Y;
                    }


                    GApp()->Post_Event(Resize);
                    if (m_Ancor & GResize_Ancor::Top || m_Ancor & GResize_Ancor::Left)
                        GApp()->Post_Event(Move);
                }
                else m_Prev_MP = Event.MP;

                break;
            }
        }
    }

    return GBasic_Window::Callback_Func(Event);
}
