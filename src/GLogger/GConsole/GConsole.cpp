#include "GConsole/GConsole.h"
#include "GApplication.h"
#include "GLog_Manager.h"


GConsole::GConsole()
    : GWindow("Groak Console", 640, 360), m_Stream_Buff(this), m_Stream(&m_Stream_Buff) {
    m_Callback_Ptr = &GConsole::Callback_Func;
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



int GConsole::Callback_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Run:
                {
                    m_Title_Bar = new GTitle_Bar(this, { m_Window.X, 32 }, { 0, m_Window.Y - 32 });

                    GEvent Render;
                    Render.Core_Message = GECore_Message::Render;
                    Render.Data_Ptr = m_Main_Window;
                    GApp()->Post_Event(Render);
                    break;
                }

                case GEWind_Message::Resize:
                {
                    GWindow::Callback_Func(Event);

                    GEvent Event;
                    Event.Type = GEType::Window;
                    Event.Data_Ptr = m_Title_Bar;

                    Event.Wind_Message = GEWind_Message::Move;
                    Event.WP = { 0, m_Window.Y - 32 };
                    Post_Event(Event);

                    Event.Wind_Message = GEWind_Message::Resize;
                    Event.WS = { m_Window.X, 32 };
                    Post_Event(Event);

                    return 1;
                }

                case GEWind_Message::Iconify:
                case GEWind_Message::Maximise:
                case GEWind_Message::Restore:
                {
                    GEvent TBEvent;
                    TBEvent.Type = GEType::Window;
                    TBEvent.Data_Ptr = m_Title_Bar;
                    TBEvent.Wind_Message = Event.Wind_Message;
                    Post_Event(TBEvent);

                    break;
                }

                case GEWind_Message::Close:
                {
                    std::cout << "Closing GConsole!" << std::endl;
                    break;
                }

                case GEWind_Message::Terminate_Thread:
                {
                    std::cout << "GConsole: Goodbye world :<" << std::endl;
                    break;
                }
            }

            break;
        }
    }

    return GWindow::Callback_Func(Event);
}
