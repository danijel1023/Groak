#include "GLog_Stream.h"
#include "GApplication.h"
#include "GConsole/GConsole.h"

#include <fstream>


static GConsole* Console = nullptr;
GLog_Stream::GLog_Stream() {
    m_File = new std::ofstream;
}

GLog_Stream::~GLog_Stream() {
    delete m_File;  //Auto close
}


void GLog_Stream::Close_GConsole() {
    GEvent Event;
    Event.Core_Message = GECore_Message::Close;
    Event.Data_Ptr = Console;
    Console = nullptr;
}

void GLog_Stream::Enable_Log(bool Enable_Log) {
    if (!m_File_Lock)
        m_Enable_Log = Enable_Log;
}



void GLog_Stream::Set_Device(GLog_Device Out, const GString& File) {
    switch (Out) {
        case GLog_Device::GConsole:
        {
            if (m_Stream_Ptr == m_File) m_File->close();
            if (!Console) {
                Console = new GConsole;
                GApp()->Register_Window(Console);
                //GApp()->Attach_Simulator(Console, true);
            }

            Console->Show();
            m_File_Lock = false;
            m_Stream_Ptr = Console->Get_Stream();

            break;
        }

        case GLog_Device::Std_Console:
        {
            if (m_Stream_Ptr == m_File) m_File->close();

            if (Console) Console->Hide();
            m_File_Lock = false;
            m_Stream_Ptr = &std::cout;

            break;
        }

        case GLog_Device::File:
        {
            m_File->open(File.c_str());
            if (Console) Console->Hide();

            if (m_File->is_open()) m_Stream_Ptr = m_File;
            else {
                m_File_Lock = true;
                m_Enable_Log = false;
            }

            break;
        }
    }

    m_Out = Out;
}

void GLog_Stream::Set_Line_Ending(GString Line_Ending) {
    m_Line_Ending = Line_Ending;
}

std::ostream* GLog_Stream::Get_Stream() {
    return m_Stream_Ptr;
}



void GLog_Stream::New_Line() {
    if (m_Enable_Log) {
        (*m_Stream_Ptr) << m_Line_Ending;
        m_Stream_Ptr->flush();
    }
}
