#include "GDecorated_Window.h"
#include "GApplication.h"

GDecorated_Window::GDecorated_Window(const GString& Name, int Window_X, int Window_Y)
    : GDecorated_Window(Name, { Window_X, Window_Y }) {}

GDecorated_Window::GDecorated_Window(const GString& Name, const GSize& Window)
    : GWindow("Groak Console", Window) {
    m_Callback_Ptr = &GDecorated_Window::Callback_Func;
}

GDecorated_Window::~GDecorated_Window() {}




int GDecorated_Window::Callback_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Run:
                {
                    m_Left_Resize = new GResize_Vertical(this, { 10, m_Window.Y - 20 }, { 0, 10 }, GResV::Left);
                    m_Right_Resize = new GResize_Vertical(this, { 10, m_Window.Y - 20 }, { m_Window.X - 10, 10 }, GResV::Right);

                    m_Up_Resize = new GResize_Horizontal(this, { m_Window.X, 10 }, { 0, m_Window.Y - 10 }, GResH::Top);
                    m_Down_Resize = new GResize_Horizontal(this, { m_Window.X, 10 }, { 0, 0 }, GResH::Bottom);

                    m_Title_Bar = new GTitle_Bar(this, { m_Window.X, 32 }, { 0, m_Window.Y - 32 });
                    break;
                }
                
                case GEWind_Message::Resize:
                {
                    GWindow::Callback_Func(Event);

                    m_Left_Resize->Set_Window({ 10, m_Window.Y - 20 });
                    m_Left_Resize->Set_Screen({ 0, 10 });
                    m_Right_Resize->Set_Window({ 10, m_Window.Y - 20 });
                    m_Right_Resize->Set_Screen({ m_Window.X - 10, 10 });

                    m_Up_Resize->Set_Window({ m_Window.X, 10 });
                    m_Up_Resize->Set_Screen({ 0, m_Window.Y - 10 });
                    m_Down_Resize->Set_Window({ m_Window.X, 10 });
                    m_Down_Resize->Set_Screen({ 0, 0 });

                    m_Title_Bar->Set_Window({ m_Window.X, 32 });
                    m_Title_Bar->Set_Screen({ 0, m_Window.Y - 32 });

                    return 1;
                }
                
                case GEWind_Message::Iconify:
                case GEWind_Message::Maximise:
                case GEWind_Message::Restore:
                {
                    GEvent TBEvent;
                    TBEvent.Type = GEType::Window;
                    TBEvent.Data_Ptr = m_Title_Bar;
                    TBEvent.Wind_Message = Event.Wind_Message;
                    Post_Event(TBEvent);

                    break;
                }
            }

            break;
        }
    }

    return GWindow::Callback_Func(Event);
}
