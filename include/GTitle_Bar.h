#pragma once
#include "GButton.h"
#include "GBasic_Window.h"


class GTitle_Bar : public GBasic_Window {
public:
    GTitle_Bar(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y);
    GTitle_Bar(GBasic_Window* Parent, const GSize& Window, const GPos& Screen);
    ~GTitle_Bar();

    bool Iconified() { return m_Iconified; }

private:
    GButton *m_Close = nullptr, *m_Iconify = nullptr, *m_Maximise = nullptr;
    bool m_Maximised = false;
    GTexture m_Maximise_Tex, m_Restore_Tex;

    size_t m_Bkg_Quad = 0;
    GColor m_Bkg_Color = { (40.0f / 100.0f), (40.0f / 100.0f), (40.0f / 100.0f), (100.0f / 100.0f) };
    bool m_Iconified = false;

    GPos m_Prev_MP;
    bool m_Track_Mouse = false;

protected:
    int Callback_Func(const GEvent& Event);

private:
    static int Callback_Func(void* _This, const GEvent& Event) {
        auto This = static_cast<GTitle_Bar*>(_This);
        return This->Callback_Func(Event);
    }
};
