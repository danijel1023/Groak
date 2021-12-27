#include "GText_Box.h"
#include "GWindow.h"

GText_Box::GText_Box(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, bool Overlay)
    : GBasic_Window(Parent, Window, Screen) {
    m_Callback_Ptr = &GText_Box::Callback_Func;
}

GText_Box::~GText_Box() {}


std::mutex& GText_Box::Get_Mutex() {
    return m_Buffer_Mutex;
}



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
                    Renderer.Clear();

                    //Default
                    GBasic_Window::Callback_Func(Event);

                    //Render text
                    std::unique_lock<std::mutex> Render_Lck(Get_Mutex());


                    //vertical scrolling (y): YScroll(0.90) * Line_Height -> scrolled 0.90 lines
                    // - effect: scaling up and down doesnt effect the

                    for (int i = m_Text_Offset.Y; i < m_Text.size(); i++) {
                        const GString& Line = m_Text[i];
                        GPos Pos = { -m_Text_Offset.X, m_Window.Y - (m_Text_Height * (i + 1)) };

                        Renderer.Draw_Str(Line, m_Text_Color.at(i), Pos, m_Text_Height, m_Console_Font);
                    }

                    Renderer.Flush();
                    return 0;
                }
            }
        }
    }


    return GBasic_Window::Callback_Func(Event);
}
