#pragma once
#include "GBasic_Window.h"

//No vers
enum class GResH { Bottom, Top };

class GResize_Horizontal : public GBasic_Window {
public:
    GResize_Horizontal(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y, GResH Ancor);
    GResize_Horizontal(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, GResH Ancor);
    ~GResize_Horizontal();

private:
    GResH m_Ancor;
    bool m_Tracking = false;
    GPos m_Prev_MP;

protected:
    int Callback_Func(const GEvent& Event);

private:
    static int Callback_Func(void* _This, const GEvent& Event) {
        auto This = static_cast<GResize_Horizontal*>(_This);
        return This->Callback_Func(Event);
    }
};
