#include "GBasic_Window.h"
#include "GWindow.h"
#include "GApplication.h"

GBasic_Window::GBasic_Window(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y, bool Overlay)
    :GBasic_Window(Parent, { Window_X, Window_Y }, { Screen_X, Screen_Y }, Overlay) {}

GBasic_Window::GBasic_Window(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, bool Overlay)
    : m_Parent(Parent), m_Window(Window), m_Screen(Screen) {

    //If this is child window - the main windows' m_Parent is always nullptr
    if (m_Parent) {
        //Get the direct GWindow pointer
        m_Main_Window = m_Parent->m_Main_Window;
        
        //Add this child window to parents' child (/overlay) window list
        if (Overlay) m_Parent->m_Overlay_Windows.push_back(this);
        else m_Parent->m_Child_Windows.push_back(this);

        //Register the absolute position from (0, 0) from main window
        m_Absolute_Screen = m_Parent->m_Absolute_Screen + Screen;
    }
}

GBasic_Window::~GBasic_Window() {
    for (auto Ch_Wnd : m_Overlay_Windows) {
        delete Ch_Wnd;
    }

    for (auto Ch_Wnd : m_Child_Windows) {
        delete Ch_Wnd;
    }
}



GFramebuffer* GBasic_Window::Create_Framebuffer() {
    return new GFramebuffer(m_Window, this);
}

size_t GBasic_Window::Add_Quad(const GQuad& Quad) {
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
    Event.Type = GEType::Renderer;
    Event.Renderer_Message = GERenderer_Message::Render;
    m_Main_Window->Post_Event(Event);
}


void GBasic_Window::Set_Screen(const GPos& Pos) {
    GEvent Event;
    Event.Type = GEType::Window;
    Event.Data_Ptr = this;
    Event.Wind_Message = GEWind_Message::Move;
    Event.WP = Pos;
    m_Main_Window->Post_Event(Event);
}

void GBasic_Window::Set_Window(const GSize& Size) {
    GEvent Event;
    Event.Type = GEType::Window;
    Event.Data_Ptr = this;
    Event.Wind_Message = GEWind_Message::Resize;
    Event.WS = Size;
    m_Main_Window->Post_Event(Event);
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
    if (!m_Enabled) return 0;

    switch (Event.Type) {
        case GEType::Mouse:
        {
            GEvent Active;
            Active.Type = GEType::Mouse;
            Active.Mouse_Message = GEMouse_Message::Active;
            if (!GCall(this, m_Callback_Ptr, Active)) return 0;

            GBasic_Window*& Mouse_Focus = m_Main_Window->m_Mouse_Focus;
            GBasic_Window*& Wind_Under_Cursor = m_Main_Window->m_Wind_Under_Cursor;

            const auto& MP = Event.MP;
            for (auto Ch_Wnd : m_Overlay_Windows) {
                //If the mouse is left form the child wondow, ignore
                if (MP.X < Ch_Wnd->m_Screen.X)
                    continue;

                //If the mouse is right form the child wondow, ignore
                if (MP.X > Ch_Wnd->m_Screen.X + Ch_Wnd->m_Window.X)
                    continue;

                //If the mouse is below form the child wondow, ignore
                if (MP.Y < Ch_Wnd->m_Screen.Y)
                    continue;

                //If the mouse is above form the child wondow, ignore
                if (MP.Y > Ch_Wnd->m_Screen.Y + Ch_Wnd->m_Window.Y)
                    continue;


                //else - the mouse is inside the child windows' box
                GEvent Child_Event;
                Child_Event = Event;
                Child_Event.MP -= Ch_Wnd->m_Screen;
                if (GCall(Ch_Wnd, m_Dispatcher_Ptr, Child_Event)) return 1;
            }

            //Same thing as with overlay windows
            for (auto Ch_Wnd : m_Child_Windows) {
                if (MP.X < Ch_Wnd->m_Screen.X)
                    continue;

                if (MP.X > Ch_Wnd->m_Screen.X + Ch_Wnd->m_Window.X)
                    continue;

                if (MP.Y < Ch_Wnd->m_Screen.Y)
                    continue;

                if (MP.Y > Ch_Wnd->m_Screen.Y + Ch_Wnd->m_Window.Y)
                    continue;

                GEvent Child_Event;
                Child_Event = Event;
                Child_Event.MP -= Ch_Wnd->m_Screen;
                if (GCall(Ch_Wnd, m_Dispatcher_Ptr, Child_Event)) return 1;
            }

            if (Mouse_Focus) {
                GEvent Corrected_Event = Event;
                Corrected_Event.MP = Event.MP + m_Absolute_Screen - Mouse_Focus->m_Absolute_Screen;
                GCall(Mouse_Focus, m_Callback_Ptr, Corrected_Event);
            }

            if (Event.Mouse_Message == GEMouse_Message::Down) {
                uint8_t& PMB = m_Main_Window->m_Pressed_Mouse_Buttons;
                uint8_t Key = static_cast<uint8_t>(Event.Mouse_Button);

                PMB |= 1 << Key;

                GEvent Gain_Focus;
                Gain_Focus.Type = GEType::Mouse;
                Gain_Focus.Mouse_Message = GEMouse_Message::Gain_Focus;
                GCall(this, m_Callback_Ptr, Gain_Focus);

                Mouse_Focus = this;
            }

            //No child window handled the event so if the mouse position
            //is inside of the current window
            if ((MP.X >= 0 && MP.Y >= 0) && (MP.X <= m_Window.X && MP.Y <= m_Window.Y)) {

                //Send Leave if some other window was under cursor before
                if (Wind_Under_Cursor != this) {
                    //Don't sent the event to nullptr
                    if (Wind_Under_Cursor) {
                        GEvent Leave;
                        Leave.Type = GEType::Mouse;
                        Leave.Mouse_Message = GEMouse_Message::Leave;
                        Leave.MP = Event.MP + m_Absolute_Screen - Wind_Under_Cursor->m_Absolute_Screen;

                        GCall(Wind_Under_Cursor, m_Callback_Ptr, Leave);
                    }

                    GEvent Enter;
                    Enter.Type = GEType::Mouse;
                    Enter.Mouse_Message = GEMouse_Message::Enter;
                    Enter.MP = Event.MP;

                    GCall(this, m_Callback_Ptr, Enter);
                    Wind_Under_Cursor = this;
                }
            }

            if (m_Main_Window->m_Pressed_Mouse_Buttons == 0 && Mouse_Focus) {
                Mouse_Focus = nullptr;
                return 1;
            }
            else
                return GCall(this, m_Callback_Ptr, Event);
        }

        case GEType::Window:
        {
            if (Event.Wind_Message == GEWind_Message::Close || Event.Wind_Message == GEWind_Message::Terminate_Thread) {
                for (auto Ch_Wnd : m_Overlay_Windows) {
                    GCall(Ch_Wnd, m_Dispatcher_Ptr, Event);
                }

                for (auto& Ch_Wnd : m_Child_Windows) {
                    GCall(Ch_Wnd, m_Dispatcher_Ptr, Event);
                }

                GCall(this, m_Callback_Ptr, Event);
            }

            else if (Event.Wind_Message == GEWind_Message::Run) {
                GCall(this, m_Callback_Ptr, Event);

                for (auto Ch_Wnd : m_Child_Windows) {
                    GCall(Ch_Wnd, m_Dispatcher_Ptr, Event);
                }
                
                for (auto Ch_Wnd : m_Overlay_Windows) {
                    GCall(Ch_Wnd, m_Dispatcher_Ptr, Event);
                }
            }

            else if (Event.Wind_Message == GEWind_Message::Render) {
                Set_Viewport();
                GCall(this, m_Callback_Ptr, Event);

                //Reverse order (I could write this: 'for (int i = m_Child_Windows.size(); i--;)' but, it's not so readable)
                for (size_t i = m_Child_Windows.size(); i > 0; i--) {
                    GCall(m_Child_Windows[i - 1], m_Dispatcher_Ptr, Event);
                }

                for (size_t i = m_Overlay_Windows.size(); i > 0; i--) {
                    GCall(m_Overlay_Windows[i - 1], m_Dispatcher_Ptr, Event);
                }
            }

            else {
                auto Wind = static_cast<GBasic_Window*>(Event.Data_Ptr);
                GCall(Wind, m_Callback_Ptr, Event);
            }

            break;
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
                    auto& Renderer = *m_Main_Window->m_Renderer;

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

        case GEType::Keyboard:
        {
            m_Main_Window->m_Modifier_Alt = Event.Modifier_Alt;
            m_Main_Window->m_Modifier_Ctrl = Event.Modifier_Ctrl;
            m_Main_Window->m_Modifier_Shift = Event.Modifier_Shift;
            break;
        }
    }

    return 1;
}
