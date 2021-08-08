#include "GLog_Manager.h"


GLog_Stream* GLog_Manager::m_Stream_Ptr = nullptr;
GLog_Level GLog_Manager::m_Log_Lvl = GLog_Level::Trace;


void GLog_Manager::Init() { m_Stream_Ptr = new GLog_Stream; }
void GLog_Manager::End()  { delete m_Stream_Ptr; m_Stream_Ptr = nullptr; }


void GLog_Manager::Set_Log_Level(GLog_Level Log_Lvl) {
    m_Log_Lvl = Log_Lvl;
}

void GLog_Manager::Set_Line_Ending(GString Line_Ending) {
    m_Stream_Ptr->Set_Line_Ending(Line_Ending);
}

void GLog_Manager::Set_Device(GLog_Device Out, const GString& File) {
    m_Stream_Ptr->Set_Device(Out, File);
}

void GLog_Manager::Close_GConsole() { m_Stream_Ptr->Close_GConsole(); }

std::ostream* GLog_Manager::Get_Stream() {
    return m_Stream_Ptr->Get_Stream();
}

GLog_Manager::GLog_Manager() {}
GLog_Manager::~GLog_Manager() {
    if (m_Use_Line_Ending) m_Stream_Ptr->New_Line();
}



GLog_Manager& GLog_Manager::NLE() {
    m_Use_Line_Ending = false;
    return (*this);
}

//Inject "Log_Lvl" marking; [Trace], [Info], etc.
GLog_Manager& GLog_Manager::GLog(GLog_Level Log_Lvl) {
    //Decide if you should continue logging or not
    if (!m_Stream_Ptr) return (*this);

    if (Log_Lvl >= m_Log_Lvl) {
        m_Stream_Ptr->Enable_Log(true);

        switch (Log_Lvl) {
            case GLog_Level::Trace:
                (*m_Stream_Ptr) << "[Trace] ";
                break;
            case GLog_Level::Info:
                (*m_Stream_Ptr) << "[Info] ";
                break;
            case GLog_Level::Warning:
                (*m_Stream_Ptr) << "[Warning] ";
                break;
            case GLog_Level::Error:
                (*m_Stream_Ptr) << "[Error] ";
                break;
            case GLog_Level::Fatal:
                (*m_Stream_Ptr) << "[Fatal] ";
                break;
            default:
                (*m_Stream_Ptr) << "[Unknown] ";
                break;
        }
    }
    else m_Stream_Ptr->Enable_Log(false);

    return (*this);
}
