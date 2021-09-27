#include "GDecorated_Window.h"
#include "GApplication.h"

GDecorated_Window::GDecorated_Window(const GString& Name, int Window_X, int Window_Y)
    : GDecorated_Window(Name, { Window_X, Window_Y }) {}

GDecorated_Window::GDecorated_Window(const GString& Name, const GSize& Window)
    : GWindow(Name, Window) {
    m_Callback_Ptr = &GDecorated_Window::Callback_Func;
}

GDecorated_Window::~GDecorated_Window() {}



GResize_Area* Resize_Area_Arr[4] = { nullptr, nullptr, nullptr, nullptr };
int GDecorated_Window::Callback_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Run:
                {
                    //m_Left_Resize = new GResize_Area(this, { 10, m_Window.Y - 20 }, { 0, 10 }, GRes::Left, { 50, 50 });
                    //m_Right_Resize = new GResize_Area(this, { 10, m_Window.Y - 20 }, { m_Window.X - 10, 10 }, GRes::Right, { 50, 50 });
                    //
                    //m_Up_Resize = new GResize_Area(this, { m_Window.X, 10 }, { 0, m_Window.Y - 10 }, GRes::Top, { 50, 50 });
                    //m_Down_Resize = new GResize_Area(this, { m_Window.X, 10 }, { 0, 0 }, GRes::Bot, { 50, 50 });

                    Resize_Area_Arr[0] = new GResize_Area(this, {10, 10}, { m_Window.X - 10, m_Window.Y - 10}, GRes::Top_Right, {50, 50});
                    Resize_Area_Arr[1] = new GResize_Area(this, {10, 10}, { 0, m_Window.Y - 10 },              GRes::Top_Left,  {50, 50});
                    Resize_Area_Arr[2] = new GResize_Area(this, {10, 10}, { 0, 0 },                            GRes::Bot_Left,  {50, 50});
                    Resize_Area_Arr[3] = new GResize_Area(this, {10, 10}, { m_Window.X - 10, 0},               GRes::Bot_Right, {50, 50});

                    //m_Title_Bar = new GTitle_Bar(this, { m_Window.X, 32 }, { 0, m_Window.Y - 32 });
                    break;
                }
                
                case GEWind_Message::Resize:
                {
                    GWindow::Callback_Func(Event);

                    //m_Left_Resize->Set_Window({ 10, m_Window.Y - 20 });
                    //m_Left_Resize->Set_Screen({ 0, 10 });
                    //m_Right_Resize->Set_Window({ 10, m_Window.Y - 20 });
                    //m_Right_Resize->Set_Screen({ m_Window.X - 10, 10 });
                    //
                    //m_Up_Resize->Set_Window({ m_Window.X, 10 });
                    //m_Up_Resize->Set_Screen({ 0, m_Window.Y - 10 });
                    //m_Down_Resize->Set_Window({ m_Window.X, 10 });
                    //m_Down_Resize->Set_Screen({ 0, 0 });

                    Resize_Area_Arr[0]->Set_Screen({ m_Window.X - 10, m_Window.Y - 10 });
                    Resize_Area_Arr[1]->Set_Screen({ 0, m_Window.Y - 10 });
                    Resize_Area_Arr[2]->Set_Screen({ 0, 0 });
                    Resize_Area_Arr[3]->Set_Screen({ m_Window.X - 10, 0 });

                    //m_Title_Bar->Set_Window({ m_Window.X, 32 });
                    //m_Title_Bar->Set_Screen({ 0, m_Window.Y - 32 });

                    return 1;
                }
                
                case GEWind_Message::Iconify:
                case GEWind_Message::Maximise:
                case GEWind_Message::Restore:
                {
                    //GEvent TBEvent;
                    //TBEvent.Type = GEType::Window;
                    //TBEvent.Data_Ptr = m_Title_Bar;
                    //TBEvent.Wind_Message = Event.Wind_Message;
                    //Post_Event(TBEvent);

                    break;
                }
            }

            break;
        }

        case GEType::Mouse:
        {
            switch (Event.Mouse_Message) {
                case GEMouse_Message::Down:
                {
                    GEvent Event;
                    Event.Data_Ptr = this;
                    Event.Core_Message = GECore_Message::Resize;
                    Event.WS = {m_Window.X, m_Window.Y + 200};
                    //GApp()->Post_Event(Event);
                }
            }

            break;
        }
    }

    return GWindow::Callback_Func(Event);
}
