#pragma once
#include "GBasic_Window.h"

enum class GResV { Left, Right };

class GResize_Vertical : public GBasic_Window {
public:
    GResize_Vertical(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y, GResV Ancor);
    GResize_Vertical(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, GResV Ancor);
    ~GResize_Vertical();

private:
    GResV m_Ancor;
    bool m_Tracking = false;

protected:
    int Callback_Func(const GEvent& Event);

private:
    static int Callback_Func(void* _This, const GEvent& Event) {
        auto This = static_cast<GResize_Vertical*>(_This);
        return This->Callback_Func(Event);
    }
};
