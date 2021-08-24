#pragma once
#include "GWindow.h"
#include "GTitle_Bar.h"
#include "GResize_Area/GResize_Horizontal.h"
#include "GResize_Area/GResize_Vertical.h"


class GDecorated_Window : public GWindow {
public:
    GDecorated_Window(const GString& Name, int Window_X, int Window_Y);
    GDecorated_Window(const GString& Name, const GSize& Window);

    ~GDecorated_Window();

private:
    GTitle_Bar* m_Title_Bar = nullptr;
    GResize_Vertical* m_Left_Resize = nullptr, * m_Right_Resize = nullptr;
    GResize_Horizontal* m_Up_Resize = nullptr, * m_Down_Resize = nullptr;

protected:
    int Callback_Func(const GEvent& Event);

private:
    static int Callback_Func(void* _This, const GEvent& Event) {
        auto This = static_cast<GDecorated_Window*>(_This);
        return This->Callback_Func(Event);
    }
};
