#include "GResize_Area/GResize_Vertical.h"

GResize_Vertical::GResize_Vertical(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y, GResV Ancor)
    : GResize_Vertical(Parent, { Window_X, Window_Y }, { Screen_X, Screen_Y }, Ancor) {}

GResize_Vertical::GResize_Vertical(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, GResV Ancor)
    : GBasic_Window(Parent, Window, Screen), m_Ancor(Ancor) {
    m_Callback_Ptr = &GResize_Vertical::Callback_Func;
}

GResize_Vertical::~GResize_Vertical() {}



int GResize_Vertical::Callback_Func(const GEvent& Event) {
    return GBasic_Window::Callback_Func(Event);
}
