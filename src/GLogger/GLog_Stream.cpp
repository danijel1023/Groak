#include "GLog_Stream.h"
#include "GConsole.h"
#include "GApplication.h"

#include <fstream>
#include <assert.h>


static GConsole* Console = nullptr;
GLog_Stream::GLog_Stream() {
    m_File = new std::ofstream;
}

GLog_Stream::~GLog_Stream() {
    delete m_File;  //Auto close
}


void GLog_Stream::Close_GConsole() {
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



void GLog_Stream::New_Line() {
    if (m_Enable_Log) {
        (*m_Stream_Ptr) << m_Line_Ending;
        m_Stream_Ptr->flush();
    }
}

GLog_Stream& GLog_Stream::operator<<(GString Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return *this;
}

GLog_Stream& GLog_Stream::operator<<(char Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return *this;
}


GLog_Stream& GLog_Stream::operator<<(bool Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(short Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(unsigned short Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(int Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(unsigned int Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(long Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(unsigned long Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(long long Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(unsigned long long Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(float Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(double Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(long double Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}


GLog_Stream& GLog_Stream::operator<<(const char* Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << GString(Right);
    return (*this);
}

GLog_Stream& GLog_Stream::operator<<(const void* Right) {
    if (m_Enable_Log) (*m_Stream_Ptr) << Right;
    return (*this);
}
