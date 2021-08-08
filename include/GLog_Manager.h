#pragma once
#include "GLog_Stream.h"



class GLog_Manager {
public:
    GLog_Manager();
    ~GLog_Manager();

    static void Init();
    static void End();
    static void Set_Line_Ending(GString Line_Ending);

    static void Set_Device(GLog_Device Out, const GString& File = "");
    static void Set_Log_Level(GLog_Level Log_Lvl);
    static void Close_GConsole();
    static std::ostream* Get_Stream();

    //No Line Ending
    GLog_Manager& NLE();
    GLog_Manager& GLog(GLog_Level Log_Lvl);

    //Pass the paramater to the GLog_Stream class
    template<typename T>
    GLog_Stream& operator<<(T Left) {
        return (*m_Stream_Ptr) << Left;
    }

private:
    static GLog_Stream* m_Stream_Ptr;
    static GLog_Level m_Log_Lvl;

    bool m_Use_Line_Ending = true;
};


#ifndef GSAFE_LOG
#define _GLog(x) GLog_Manager().GLog(x)

#define GFatal()      _GLog(GLog_Level::Fatal)
#define GError()      _GLog(GLog_Level::Error)
#define GWarning()    _GLog(GLog_Level::Warning)
#define GInfo()       _GLog(GLog_Level::Info)
#define GTrace()      _GLog(GLog_Level::Trace)
#endif // GSAFE_LOG
