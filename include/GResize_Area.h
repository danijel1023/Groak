#pragma once
#include "GBasic_Window.h"

enum class GResize_Ancor {
    Bot = 0x01,
    Top = 0x02,
    Left = 0x04,
    Right = 0x08,

    Bot_Left = 0x01 | 0x04,
    Bot_Right = 0x01 | 0x08,
    Top_Left = 0x02 | 0x04,
    Top_Right = 0x02 | 0x08
};


class GResize_Area : public GBasic_Window {
public:
    GResize_Area(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y, GResize_Ancor Ancor, int Min_Size_X = 1, int Min_Size_Y = 1, int Max_Size_X = (uint16_t)-1, int Max_Size_Y = (uint16_t)-1, bool Overlay = false);
    GResize_Area(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, GResize_Ancor Ancor, GSize Min_Size = { 1, 1 }, GSize Max_Size = { (uint16_t)-1, (uint16_t)-1 }, bool Overlay = false);
    ~GResize_Area();

private:
    GSize m_Min_Size, m_Max_Size;
    GResize_Ancor m_Ancor;

    bool m_Tracking = false;
    GPos m_Prev_MP;

protected:
    int Callback_Func(const GEvent& Event);

private:
    static int Callback_Func(void* _This, const GEvent& Event) {
        auto This = static_cast<GResize_Area*>(_This);
        return This->Callback_Func(Event);
    }
};
