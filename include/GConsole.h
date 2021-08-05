#pragma once
#include "GWindow.h"
#include <sstream>

class GConsole;
class GSBuff : public std::stringbuf {
public:
    GSBuff(GConsole* Console);
    int sync() override;

private:
    GConsole* m_Console;
};


/*
* If you close GConsole with close signal, then the actuall GConsole object will get destroied.
* But if you just change the `Log_Device`, then the console stays open and just gets hidden.
* -- It's not a but, it's a feature
*/

class GConsole : public GWindow {
public:
    GConsole();
    ~GConsole();

    // Controls
    void Show();
    void Hide();
    std::ostream* Get_Stream();

private:
    friend class GSBuff;
    GSBuff m_Stream_Buff;
    std::ostream m_Stream;

    std::atomic_bool m_Read = false;
    std::vector<GString> m_Buffer;


protected:
    int Callback_Func(const GEvent& Event);

private:
    static int Callback_Func(void* _This, const GEvent& Event) {
        auto This = static_cast<GConsole*>(_This);
        return This->Callback_Func(Event);
    }

};
