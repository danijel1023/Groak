#include "GResize_Area/GResize_Horizontal.h"
#include "GApplication.h"
#include "GWindow.h"

GResize_Horizontal::GResize_Horizontal(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y, GResH Ancor)
    : GResize_Horizontal(Parent, { Window_X, Window_Y }, { Screen_X, Screen_Y }, Ancor) {}

GResize_Horizontal::GResize_Horizontal(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, GResH Ancor)
    : GBasic_Window(Parent, Window, Screen), m_Ancor(Ancor) {
    m_Callback_Ptr = &GResize_Horizontal::Callback_Func;
}

GResize_Horizontal::~GResize_Horizontal() {}



int GResize_Horizontal::Callback_Func(const GEvent& Event) {
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
                std::cout << "Resize Horizontal Mouse " << ((m_Ancor == GResH::Top) ? "Top" : "Bottom") << std::endl;
                //GApp()->Resolve_Event(Event, &std::cout);
                if (m_Tracking) {
                    //if (m_Ancor == GResH::Top) {
                    //    GEvent Move;
                    //    Move.Data_Ptr = m_Main_Window;
                    //    Move.Core_Message = GECore_Message::Move;
                    //
                    //    Move.WP = m_Main_Window->Get_Screen();
                    //    Move.WP.Y -= Event.MP.Y - m_Prev_MP.Y;
                    //    GApp()->Send_Event(Move);
                    //}

                    GEvent Resize;
                    Resize.Data_Ptr = m_Main_Window;
                    Resize.Core_Message = GECore_Message::Resize;

                    Resize.WS = m_Main_Window->Get_Window();
                    Resize.WS.Y -= Event.MP.Y - m_Prev_MP.Y;
                    GApp()->Post_Event(Resize);

                    //m_Prev_MP = Event.MP;
                }
                else m_Prev_MP = Event.MP;

                break;
            }
        }
    }

    return GBasic_Window::Callback_Func(Event);
}
