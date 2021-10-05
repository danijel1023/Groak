#include "GDecorated_Window.h"
#include "GApplication.h"

GDecorated_Window::GDecorated_Window(const GString& Name, int Window_X, int Window_Y)
    : GDecorated_Window(Name, { Window_X, Window_Y }) {}

GDecorated_Window::GDecorated_Window(const GString& Name, const GSize& Window)
    : GWindow(Name, Window) {
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
                    m_Left  = new GResize_Area(this, { 7, m_Window.Y - 14 }, { 0, 7 },              GResize_Ancor::Left,  m_Min_Size, m_Max_Size, true);
                    m_Right = new GResize_Area(this, { 7, m_Window.Y - 14 }, { m_Window.X - 7, 7 }, GResize_Ancor::Right, m_Min_Size, m_Max_Size, true);
                    
                    m_Top = new GResize_Area(this, { m_Window.X - 14, 7 }, { 7, m_Window.Y - 7 }, GResize_Ancor::Top, m_Min_Size, m_Max_Size, true);
                    m_Bot = new GResize_Area(this, { m_Window.X - 14, 7 }, { 7, 0 },              GResize_Ancor::Bot, m_Min_Size, m_Max_Size, true);

                    m_Top_Left  = new GResize_Area(this, {7, 7}, { 0, m_Window.Y - 7 },             GResize_Ancor::Top_Left,  m_Min_Size, m_Max_Size, true);
                    m_Top_Right = new GResize_Area(this, {7, 7}, { m_Window.X - 7, m_Window.Y - 7}, GResize_Ancor::Top_Right, m_Min_Size, m_Max_Size, true);
                    m_Bot_Left  = new GResize_Area(this, {7, 7}, { 0, 0 },                          GResize_Ancor::Bot_Left,  m_Min_Size, m_Max_Size, true);
                    m_Bot_Right = new GResize_Area(this, {7, 7}, { m_Window.X - 7, 0},              GResize_Ancor::Bot_Right, m_Min_Size, m_Max_Size, true);

                    m_Title_Bar = new GTitle_Bar(this, { m_Window.X, 32 }, { 0, m_Window.Y - 32 });
                    break;
                }
                
                case GEWind_Message::Resize:
                {
                    GWindow::Callback_Func(Event);

                    m_Left ->Set_Window({ 7, m_Window.Y - 14 });
                    m_Right->Set_Window({ 7, m_Window.Y - 14 });
                    m_Left ->Set_Screen({ 0, 7 });
                    m_Right->Set_Screen({ m_Window.X - 7, 7 });
                    
                    m_Top->Set_Window({ m_Window.X - 14, 7 });
                    m_Bot->Set_Window({ m_Window.X - 14, 7 });
                    m_Top->Set_Screen({ 7, m_Window.Y - 7 });
                    m_Bot->Set_Screen({ 7, 0 });

                    m_Top_Left ->Set_Screen({ 0, m_Window.Y - 7 });
                    m_Top_Right->Set_Screen({ m_Window.X - 7, m_Window.Y - 7 });
                    m_Bot_Left ->Set_Screen({ 0, 0 });
                    m_Bot_Right->Set_Screen({ m_Window.X - 7, 0 });

                    m_Title_Bar->Set_Window({ m_Window.X, 32 });
                    m_Title_Bar->Set_Screen({ 0, m_Window.Y - 32 });

                    return 1;
                }
                
                case GEWind_Message::Iconify:
                case GEWind_Message::Maximise:
                case GEWind_Message::Restore:
                {
                    if (Event.Wind_Message == GEWind_Message::Maximise) {
                        m_Left      ->m_Enabled = false;
                        m_Right     ->m_Enabled = false;
                        m_Top       ->m_Enabled = false;
                        m_Bot       ->m_Enabled = false;
                        m_Top_Left  ->m_Enabled = false;
                        m_Top_Right ->m_Enabled = false;
                        m_Bot_Left  ->m_Enabled = false;
                        m_Bot_Right ->m_Enabled = false;
                    }

                    else {
                        m_Left      ->m_Enabled = true;
                        m_Right     ->m_Enabled = true;
                        m_Top       ->m_Enabled = true;
                        m_Bot       ->m_Enabled = true;
                        m_Top_Left  ->m_Enabled = true;
                        m_Top_Right ->m_Enabled = true;
                        m_Bot_Left  ->m_Enabled = true;
                        m_Bot_Right ->m_Enabled = true;
                    }

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


void GDecorated_Window::Set_Min_Size(const GSize& Size) {
    m_Min_Size = Size;
}

void GDecorated_Window::Set_Max_Size(const GSize& Size) {
    m_Max_Size = Size;
}

GSize GDecorated_Window::Get_Min_Size() {
    return m_Min_Size;
}

GSize GDecorated_Window::Get_Max_Size() {
    return m_Max_Size;
}
