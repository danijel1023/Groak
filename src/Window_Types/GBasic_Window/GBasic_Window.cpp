#include "GBasic_Window.h"
#include "GWindow.h"
#include "GApplication.h"

GBasic_Window::GBasic_Window(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y)
    :GBasic_Window(Parent, { Window_X, Window_Y }, { Screen_X, Screen_Y }) {}

GBasic_Window::GBasic_Window(GBasic_Window* Parent, const GSize& Window, const GPos& Screen)
    : m_Parent(Parent), m_Window(Window), m_Screen(Screen) {

    //If this is child window - the main windows' m_Parent is always nullptr
    if (m_Parent) {
        //Get the direct GWindow pointer
        m_Main_Window = m_Parent->m_Main_Window;
        
        //Add this child window to parents' child window list
        m_Parent->m_Child_Windows.push_back(this);

        //Register the absolute position from (0, 0) from main window
        m_Absolute_Screen = m_Parent->m_Absolute_Screen + Screen;
    }
}

GBasic_Window::~GBasic_Window() {
    for (auto Ch_Wnd : m_Child_Windows) {
        delete Ch_Wnd;
    }
}



GFramebuffer* GBasic_Window::Create_Framebuffer() {
    return new GFramebuffer(m_Window, this);
}

size_t GBasic_Window::Add_Quad(GQuad& Quad) {
    m_Quad_List.push_back(Quad);
    return m_Quad_List.size() - 1;
}

GQuad& GBasic_Window::Get_Quad(size_t Quad_i) {
    return m_Quad_List[Quad_i];
}


void GBasic_Window::Set_Viewport() {
    auto& Renderer = *(m_Main_Window->m_Renderer);
    Renderer.Set_Window_Space(m_Absolute_Screen - m_Main_Window->m_FB_Ancor, m_Window);
}


void GBasic_Window::Render() {
    GEvent Event;
    Event.Core_Message = GECore_Message::Render;
    Event.Data_Ptr = m_Main_Window;
    GApp()->Post_Event(Event);
}



int GBasic_Window::Dispatcher_Func(void* _This, const GEvent& Event) {
    auto This = static_cast<GBasic_Window*>(_This);
    return This->Dispatcher_Func(Event);
}

int GBasic_Window::Callback_Func(void* _This, const GEvent& Event) {
    auto This = static_cast<GBasic_Window*>(_This);
    return This->Callback_Func(Event);
}

int GBasic_Window::Dispatcher_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Mouse:
        {
            GEvent Active;
            Active.Type = GEType::Mouse;
            Active.Mouse_Message = GEMouse_Message::Active;
            if (!GCall(this, m_Callback_Ptr, Active)) return 0;

            auto& MP = Event.MP;
            for (auto Ch_Wnd : m_Child_Windows) {
                //If the mouse is left form the child wondow, ignore
                if (MP.X < Ch_Wnd->m_Screen.X) {
                    continue;
                }

                //If the mouse is right form the child wondow, ignore
                if (MP.X > Ch_Wnd->m_Screen.X + Ch_Wnd->m_Window.X) {
                    continue;
                }


                //If the mouse is below form the child wondow, ignore
                if (MP.Y < Ch_Wnd->m_Screen.Y) {
                    continue;
                }

                //If the mouse is above form the child wondow, ignore
                if (MP.Y > Ch_Wnd->m_Screen.Y + Ch_Wnd->m_Window.Y) {
                    continue;
                }

                //else - the mouse is inside the child windows' box
                GEvent Child_Event;
                Child_Event = Event;
                Child_Event.MP -= Ch_Wnd->m_Screen;
                if (GCall(Ch_Wnd, m_Dispatcher_Ptr, Child_Event)) return 1;
            }


            if (Event.Mouse_Message == GEMouse_Message::Down) {
                m_Main_Window->m_Mouse_Buttons_Pressed++;
                m_Main_Window->m_Mouse_Focus = this;
            }

            //No child window handled the event so this window will now process the event
            if ((MP.X >= 0 && MP.Y >= 0) && (MP.X <= m_Window.X && MP.Y <= m_Window.Y)) {
                if (m_Main_Window->m_Wind_Under_Cursor != this) {
                    GBasic_Window*& Wind_Under_Cursor = m_Main_Window->m_Wind_Under_Cursor;

                    if (Wind_Under_Cursor) {
                        GCall(Wind_Under_Cursor, m_Callback_Ptr, Event);

                        GEvent Leave;
                        Leave.Type = GEType::Mouse;
                        Leave.Mouse_Message = GEMouse_Message::Leave;
                        Leave.MP = Event.MP;

                        GCall(Wind_Under_Cursor, m_Callback_Ptr, Leave);
                    }

                    GEvent Enter;
                    Enter.Type = GEType::Mouse;
                    Enter.Mouse_Message = GEMouse_Message::Enter;
                    Enter.MP = Event.MP;

                    GCall(this, m_Callback_Ptr, Enter);

                    if (!Wind_Under_Cursor) GCall(this, m_Callback_Ptr, Event);
                    Wind_Under_Cursor = this;
                }
            }

            if (m_Main_Window->m_Wind_Under_Cursor)
                return GCall(m_Main_Window->m_Wind_Under_Cursor, m_Callback_Ptr, Event);
            else
                return GCall(this, m_Callback_Ptr, Event);
        }

        case GEType::Window:
        {
            if (Event.Wind_Message == GEWind_Message::Close || Event.Wind_Message == GEWind_Message::Terminate_Thread) {
                for (auto& Ch_Wnd : m_Child_Windows) {
                    GCall(Ch_Wnd, m_Dispatcher_Ptr, Event);
                }

                GCall(this, m_Callback_Ptr, Event);
            }

            else if (Event.Wind_Message == GEWind_Message::Run || Event.Wind_Message == GEWind_Message::Render) {
                GCall(this, m_Callback_Ptr, Event);

                for (auto Ch_Wnd : m_Child_Windows) {
                    GCall(Ch_Wnd, m_Dispatcher_Ptr, Event);
                }
            }

            else {
                auto Wind = static_cast<GBasic_Window*>(Event.Data_Ptr);
                GCall(Wind, m_Callback_Ptr, Event);
            }
        }
    }

    return 0;
}


int GBasic_Window::Callback_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Render:
                {
                    auto& Renderer = *(m_Main_Window->m_Renderer);

                    Set_Viewport();
                    for (auto& Quad : m_Quad_List) if (Quad.m_Active) Renderer.Add_Quad(Quad);
                    Renderer.Flush();

                    return 1;
                }

                case GEWind_Message::Move:
                {
                    m_Screen = Event.WP;
                    m_Absolute_Screen = m_Parent->m_Absolute_Screen + m_Screen;
                    break;
                }

                case GEWind_Message::Resize:
                {
                    m_Window = Event.WS;
                    break;
                }
            }

            break;
        }



        //[TODO] This is for Child Window
        //case EType::Mouse:
        //{
        //    switch (*static_cast<EMouse_Message*>(Event.Message)) {
        //        case EMouse_Message::LMB_Down:
        //        {
        //            m_Main_hwnd->m_Focus = this;
        //            break;
        //        }
        //    }
        //
        //    break;
        //}
    }

    return 1;
}
