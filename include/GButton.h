#include "GBasic_Window.h"
#include "GTexture.h"


class GButton : public GBasic_Window {
public:
    GButton(GBasic_Window* Parent, const GSize& Window, const GPos& Screen);
    ~GButton();

    GQuad& Get_Color_Quad();
    GQuad& Get_Texture_Quad();

    GTexture m_Texture;
    GColor m_Hover, m_Press, m_Leave;
    GEvent m_On_Press_Event;

private:
    size_t m_Color_Quad = 0;
    size_t m_Texture_Quad = 0;
    bool m_Pressed = false;

protected:
    int Callback_Func(const GEvent& Event);

private:
    static int Callback_Func(void* _This, const GEvent& Event) {
        auto This = static_cast<GButton*>(_This);
        return This->Callback_Func(Event);
    }
};
