#include "GConsole/GConsole.h"
#include "GApplication.h"
#include "GLog_Manager.h"


GConsole::GConsole()
    : GDecorated_Window("Groak Console", 50, 50), m_Stream_Buff(this), m_Stream(&m_Stream_Buff) {
    m_Callback_Ptr = &GConsole::Callback_Func;
    //GApp()->Attach_Simulator(this, false);
}

GConsole::~GConsole() {
    GLog_Manager::Close_GConsole();
    GLog_Manager::Set_Device(GLog_Device::Std_Console);
}



void GConsole::Show() {
    GEvent Event;
    Event.Core_Message = GECore_Message::Show;
    Event.Data_Ptr = this;
    GApp()->Post_Event(Event);
}

void GConsole::Hide() {
    GEvent Event;
    Event.Core_Message = GECore_Message::Hide;
    Event.Data_Ptr = this;
    GApp()->Post_Event(Event);
}

std::ostream* GConsole::Get_Stream() { return &m_Stream; }

GPos MP_C;
bool Track_C = false;

int GConsole::Callback_Func(const GEvent& Event) {
    //return GDecorated_Window::Callback_Func(Event);

    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Run:
                {
                    GQuad Quad({ 2, 100 }, { 20, 0 });
                    Quad.m_Color = { 1.0f, 0.0f, 0.0f, 1.0f };
                    //Add_Quad(Quad);

                    GEvent Render;
                    //Render.Core_Message = GECore_Message::Render;
                    Render.Data_Ptr = m_Main_Window;
                    //GApp()->Post_Event(Render);
                    break;
                }

                //case GEWind_Message::Resize:
                //{
                //    GDecorated_Window::Callback_Func(Event);
                //
                //    GEvent Event;
                //    Event.Type = GEType::Window;
                //    Event.Data_Ptr = m_Title_Bar;
                //
                //    Event.Wind_Message = GEWind_Message::Move;
                //    Event.WP = { 0, m_Window.Y - 32 };
                //    Post_Event(Event);
                //
                //    Event.Wind_Message = GEWind_Message::Resize;
                //    Event.WS = { m_Window.X, 32 };
                //    Post_Event(Event);
                //
                //    return 1;
                //}
                //
                //case GEWind_Message::Iconify:
                //case GEWind_Message::Maximise:
                //case GEWind_Message::Restore:
                //{
                //    GEvent TBEvent;
                //    TBEvent.Type = GEType::Window;
                //    TBEvent.Data_Ptr = m_Title_Bar;
                //    TBEvent.Wind_Message = Event.Wind_Message;
                //    Post_Event(TBEvent);
                //
                //    break;
                //}
                //
                //case GEWind_Message::Close:
                //{
                //    std::cout << "Closing GConsole!" << std::endl;
                //    break;
                //}
                //
                //case GEWind_Message::Terminate_Thread:
                //{
                //    std::cout << "GConsole: Goodbye world :<" << std::endl;
                //    break;
                //}
            }

            break;
        }

        case GEType::Keyboard:
        {
            //256
            if (Event.Keyboard_Message == GEKeyboard_Message::Key) {
                if (Event.Key_Action == GEKey_Action::Down && Event.Key == 256) {

                    GEvent Event;
                    Event.Core_Message = GECore_Message::Terminate;
                    GApp()->Post_Event(Event);
                }
            }

            break;
        }


        case GEType::Mouse:
        {
            if (Event.Mouse_Message == GEMouse_Message::Move) {
                if (Track_C) {
                    GEvent Move_E;
                    Move_E.Data_Ptr = m_Main_Window;
                    Move_E.Core_Message = GECore_Message::Move;

                    Move_E.WP = m_Screen;
                    Move_E.WP.X += Event.MP.X - MP_C.X;
                    Move_E.WP.Y -= Event.MP.Y - MP_C.Y;
                    GApp()->Post_Event(Move_E);
                }
                else MP_C = Event.MP;
            }

            else if (Event.Mouse_Button == GEMouse_Button::LMB) {
                if (Event.Mouse_Message == GEMouse_Message::Down) {
                    Track_C = true;
                    Set_Focus(this);
                }

                else if (Event.Mouse_Message == GEMouse_Message::Up) {
                    Track_C = false;
                }
            }

            return 1;
        }
    }

    return GDecorated_Window::Callback_Func(Event);
}
