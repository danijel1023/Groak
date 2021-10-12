#pragma once
#include "GConsole/GSBuff.h"
#include "GConsole/Content_Area.h"

#include "GTitle_Bar.h"
#include "GDecorated_Window.h"
#include <atomic>

/*
* If you close GConsole with close signal, then the actuall GConsole object will get destroied.
* But if you just change the `Log_Device`, then the console stays open and just gets hidden.
* -- It's not a bug, it's a feature
*/

class GConsole : public GDecorated_Window {
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

    std::mutex m_Buffer_Mutex, m_Render_Sync_Mutex;
    std::vector<GString> m_Buffer;
    std::vector<GText> m_Render_Text;

    GFont* m_;

protected:
    int Callback_Func(const GEvent& Event);

private:
    static int Callback_Func(void* _This, const GEvent& Event) {
        auto This = static_cast<GConsole*>(_This);
        return This->Callback_Func(Event);
    }
};
