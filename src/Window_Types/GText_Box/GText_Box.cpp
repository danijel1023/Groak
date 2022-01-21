#include "GText_Box.h"
#include "GWindow.h"

//Has Fractional Part
#define HFP(x) ((x) != int64_t(x))
//Get Fractional Part
#define GFP(x) ((x) - int64_t(x))


GText_Box::GText_Box(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, bool Overlay)
    : GBasic_Window(Parent, Window, Screen, Overlay) {
    m_Callback_Ptr = &GText_Box::Callback_Func;
}

GText_Box::~GText_Box() {}

int GText_Box::Callback_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Run:
                {
                    break;
                }

                case GEWind_Message::Close:
                {
                    break;
                }

                case GEWind_Message::Render:
                {
                    GRenderer& Renderer = *m_Main_Window->Get_Renderer();
                    Set_Viewport();


                    //Default
                    GBasic_Window::Callback_Func(Event);

                    //Render text
                    std::unique_lock<std::mutex> Render_Lck(m_Buffer_Mutex);

                    for (int i = int(m_Text_Offset.Y); i < int(m_Text.size()); i++) {
                        if ((i - m_Text_Offset.Y) * m_Text_Height > m_Window.Y) break;
                        if (i < 0) continue;

                        int Y = int(m_Text_Height * (float(i) - m_Text_Offset.Y));
                        Renderer.Draw_Str(m_Text[i], m_Text_Color.at(i), { int(m_Text_Offset.X), int(m_Window.Y - m_Text_Height - Y) }, m_Text_Height, m_Console_Font);
                    }

                    Renderer.Flush();
                    return 1;
                }
            }

            break;
        }

        case GEType::Mouse:
        {
            if (Event.Mouse_Message == GEMouse_Message::Scroll_Down
                || Event.Mouse_Message == GEMouse_Message::Scroll_Up) {
                return 0;
            }

            break;
        }
    }


    return GBasic_Window::Callback_Func(Event);
}
