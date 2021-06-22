#pragma once
#include "GString.h"
#include <iostream>


enum class GLog_Device {
    Null, Std_Console, GConsole, File
};

enum class GLog_Level {
    Trace, Info, Warning, Error, Fatal, Null
};



class GLog_Stream {
public:
    GLog_Stream();
    ~GLog_Stream();

    void Enable_Log(bool Enable_Log);
    void Set_Device(GLog_Device Out);
    void Set_Line_Ending(GString Line_Ending);

    void New_Line();
    GLog_Stream& operator<<(GString Right);
    GLog_Stream& operator<<(char Right);

    GLog_Stream& operator<<(bool Right);
    GLog_Stream& operator<<(short Right);
    GLog_Stream& operator<<(unsigned short Right);
    GLog_Stream& operator<<(int Right);
    GLog_Stream& operator<<(unsigned int Right);
    GLog_Stream& operator<<(long Right);
    GLog_Stream& operator<<(unsigned long Right);
    GLog_Stream& operator<<(long long Right);
    GLog_Stream& operator<<(unsigned long long Right);
    GLog_Stream& operator<<(float Right);
    GLog_Stream& operator<<(double Right);
    GLog_Stream& operator<<(long double Right);
    GLog_Stream& operator<<(const char* Right);
    GLog_Stream& operator<<(const void* Right);

private:
    GString m_Line_Ending = "\n";
    GLog_Device m_Out = GLog_Device::Null;

    std::ofstream* m_File = nullptr;
    std::ostream* m_Stream_Ptr = nullptr;

    bool m_Enable_Log = true;
    bool m_File_Lock = false;
};
