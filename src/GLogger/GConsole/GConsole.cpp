#include "GConsole.h"
#include "GApplication.h"
#include "GLog_Manager.h"

GSBuff::GSBuff(GConsole* Console) : m_Console(Console) {}
int GSBuff::sync() {
    while (m_Console->m_Read);
    m_Console->m_Read = true;
    
    m_Console->m_Buffer.push_back(str());
    str("");

    m_Console->m_Read = false;
    return 0;
}


std::ostream* GConsole::Get_Stream() { return &m_Stream; }
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

GConsole::GConsole()
    : GWindow("Groak Console", 640, 360), m_Stream_Buff(this), m_Stream(&m_Stream_Buff) {
    m_Callback_Ptr = &GConsole::Callback_Func;
}

GConsole::~GConsole() {
    GLog_Manager::Close_GConsole();
    GLog_Manager::Set_Device(GLog_Device::Std_Console);
}



int GConsole::Callback_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Run:
                {


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
