#pragma once
#include "GString.h"
#include <iostream>


enum class GLog_Device {
    Std_Console, GConsole, File
};

enum class GLog_Level {
    Trace, Info, Warning, Error, Fatal, Null
};



class GLog_Stream {
public:
    GLog_Stream();
    ~GLog_Stream();

    void Close_GConsole();
    void Enable_Log(bool Enable_Log);
    void Set_Device(GLog_Device Out, const GString& File);
    void Set_Line_Ending(GString Line_Ending);
    std::ostream* Get_Stream();

    void New_Line();

    template<class T>
    GLog_Stream& operator<<(T Right) {
        if (m_Enable_Log) (*m_Stream_Ptr) << Right;
        return (*this);
    }

private:
    GString m_Line_Ending = "\n";
    GLog_Device m_Out = GLog_Device::Std_Console;

    std::ofstream* m_File = nullptr;
    std::ostream* m_Stream_Ptr = nullptr;

    bool m_Enable_Log = true;
    bool m_File_Lock = false;
};
