#include "GApplication.h"
#include "GWindow.h"
#include "GLog_Manager.h"


GApplication* GApplication::m_Instance = nullptr;

GApplication::GApplication() {
    GLog_Manager::Init();
    GLog_Manager::Set_Device(GLog_Device::Std_Console);
    GLog_Manager::Set_Log_Level(GLog_Level::Trace);

    m_Instance = this;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    FT_Init_FreeType(&m_FreeType);
}

GApplication::~GApplication() {
    GLog_Manager::End();
    FT_Done_FreeType(m_FreeType);
}


FT_Library& GApplication::Get_FT_Lib() {
    return m_FreeType;
}



void GApplication::Attach_Simulator(GWindow* Window, bool Recording) {
    m_Simulator_Window = Window;
    m_Recording = Recording;


    if (Recording) {
        m_Simulator_File.open("Events.bin", std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    }
    else {
        m_Simulator_File.open("Events.bin", std::ofstream::in | std::ifstream::binary);
        m_Simulator_Thread = std::thread(&GApplication::Simulator_Thread, this);
    }
}



void GApplication::Set_Context(GWindow* Window) {
    if (Current_Context != Window)
        glfwMakeContextCurrent(Window->m_Window_Hndl);
    Current_Context = Window;
}


void GApplication::Attach_Callbacks(GWindow* Window) {
    if (Window == m_Simulator_Window) {
        if (!m_Recording) return;
        else {
            glfwSetWindowUserPointer(Window->m_Window_Hndl, Window);

            glfwSetKeyCallback(Window->m_Window_Hndl, GLFW_Recording_Key_Callback);
            glfwSetCharCallback(Window->m_Window_Hndl, GLFW_Recording_Char_Callback);

            glfwSetCursorPosCallback(Window->m_Window_Hndl, GLFW_Recording_Cursor_Pos_Callback);
            glfwSetCursorEnterCallback(Window->m_Window_Hndl, GLFW_Recording_Cursor_Enter_Callback);
            glfwSetMouseButtonCallback(Window->m_Window_Hndl, GLFW_Recording_Mouse_Button_Callback);
            glfwSetScrollCallback(Window->m_Window_Hndl, GLFW_Recording_Scroll_Callback);

            glfwSetWindowSizeCallback(Window->m_Window_Hndl, GLFW_Recording_Window_Size_Callback);
            glfwSetWindowPosCallback(Window->m_Window_Hndl, GLFW_Recording_Window_Pos_Callback);
            glfwSetWindowFocusCallback(Window->m_Window_Hndl, GLFW_Recording_Window_Focus_Callback);
            glfwSetWindowCloseCallback(Window->m_Window_Hndl, GLFW_Recording_Window_Close_Callback);
            glfwSetWindowMaximizeCallback(Window->m_Window_Hndl, GLFW_Recording_Window_Maximize_Callback);
            glfwSetWindowIconifyCallback(Window->m_Window_Hndl, GLFW_Recording_Window_Iconify_Callback);
        }
    }

    else {
        glfwSetWindowUserPointer(Window->m_Window_Hndl, Window);

        glfwSetKeyCallback(Window->m_Window_Hndl, GLFW_Key_Callback);
        glfwSetCharCallback(Window->m_Window_Hndl, GLFW_Char_Callback);

        glfwSetCursorPosCallback(Window->m_Window_Hndl, GLFW_Cursor_Pos_Callback);
        glfwSetCursorEnterCallback(Window->m_Window_Hndl, GLFW_Cursor_Enter_Callback);
        glfwSetMouseButtonCallback(Window->m_Window_Hndl, GLFW_Mouse_Button_Callback);
        glfwSetScrollCallback(Window->m_Window_Hndl, GLFW_Scroll_Callback);

        glfwSetWindowSizeCallback(Window->m_Window_Hndl, GLFW_Window_Size_Callback);
        glfwSetWindowPosCallback(Window->m_Window_Hndl, GLFW_Window_Pos_Callback);
        glfwSetWindowFocusCallback(Window->m_Window_Hndl, GLFW_Window_Focus_Callback);
        glfwSetWindowCloseCallback(Window->m_Window_Hndl, GLFW_Window_Close_Callback);
        glfwSetWindowMaximizeCallback(Window->m_Window_Hndl, GLFW_Window_Maximize_Callback);
        glfwSetWindowIconifyCallback(Window->m_Window_Hndl, GLFW_Window_Iconify_Callback);
    }
}


void GApplication::Register_Window(GWindow* Window) {
    GEvent Event;
    Event.Core_Message = GECore_Message::Register;
    Event.Data_Ptr = Window;

    Post_Event(Event);
}



void GApplication::Post_Event(const GEvent& Event) {
    auto Node = m_Queue.Get_Node();
    Node->Data = Event;
    m_Queue.Insert(Node);

    glfwPostEmptyEvent();
}

void GApplication::Send_Event(const GEvent& Event) {
    {
        std::unique_lock<std::recursive_mutex> Lck(m_QRM);

        m_SE = true;
        m_SEEvent = &Event;
        glfwPostEmptyEvent();
    }

    {
        std::unique_lock<std::recursive_mutex> Lck(m_SERM);
        m_SECV.wait(Lck, [=] { return m_SE_Continue; });    //'main' thread calculating - worker waiting

        m_SE_Continue = false;
    }
}

void GApplication::Send_Event_NL(const GEvent& Event) {
    std::unique_lock<std::recursive_mutex> Lck(m_QRM);
    Worker(Event);
}



void GApplication::Worker(const GEvent& Event) {
    switch (Event.Core_Message) {
        case GECore_Message::Register:
        {
            GWindow* Window = static_cast<GWindow*>(Event.Data_Ptr);
            m_Window_List.push_back(Window);
            Window->Run();

            break;
        }

        case GECore_Message::Render:
        {
            GWindow* Window = static_cast<GWindow*>(Event.Data_Ptr);
            Set_Context(Window);

            Window->m_Renderer->Clear();

            GEvent Event;
            Event.Type = GEType::Window;
            Event.Wind_Message = GEWind_Message::Render;
            Window->Send_Event(Event);

            glfwSwapBuffers(Window->m_Window_Hndl);

            break;
        }

        case GECore_Message::Close:
        {
            for (int i = 0; i < m_Window_List.size(); i++) {
                auto& Window = m_Window_List[i];

                if (Window == static_cast<GWindow*>(Event.Data_Ptr)) {
                    auto Hndl = Window->m_Window_Hndl;

                    Window->Terminate();
                    delete Window;
                    
                    glfwDestroyWindow(Hndl);

                    m_Window_List.erase(m_Window_List.begin() + i);
                    break;
                }
            }

            break;
        }

        case GECore_Message::Terminate:
        {
            m_Running = false;
            for (auto& Window : m_Window_List) {
                GEvent Event;
                Event.Core_Message = GECore_Message::Close;
                Event.Data_Ptr = Window;
                Send_Event_NL(Event);
            }

            glfwTerminate();
            break;
        }

        case GECore_Message::Move:
        {
            GWindow* Window = static_cast<GWindow*>(Event.Data_Ptr);
            glfwSetWindowPos(Window->m_Window_Hndl, Event.WP.X, Event.WP.Y);
            break;
        }

        case GECore_Message::Show:
        {
            GWindow* Window = static_cast<GWindow*>(Event.Data_Ptr);
            glfwShowWindow(Window->m_Window_Hndl);
            break;
        }

        case GECore_Message::Hide:
        {
            GWindow* Window = static_cast<GWindow*>(Event.Data_Ptr);
            glfwHideWindow(Window->m_Window_Hndl);
            break;
        }

        case GECore_Message::Iconify:
        {
            GWindow* Window = static_cast<GWindow*>(Event.Data_Ptr);
            glfwIconifyWindow(Window->m_Window_Hndl);
            break;
        }

        case GECore_Message::Maximise:
        {
            GWindow* Window = static_cast<GWindow*>(Event.Data_Ptr);
            glfwMaximizeWindow(Window->m_Window_Hndl);
            break;
        }

        case GECore_Message::Restore:
        {
            GWindow* Window = static_cast<GWindow*>(Event.Data_Ptr);
            glfwRestoreWindow(Window->m_Window_Hndl);
            break;
        }
    }
}


int GApplication::Run() {
    int _Ret = 0;

    while (m_Running) {
        glfwWaitEvents();
        std::unique_lock<std::recursive_mutex> Lck(m_QRM);

        if (m_SE) {
            m_SE = false;
            Worker(*m_SEEvent);
            m_SE_Continue = true;
            m_SECV.notify_all();
        }

        else while (!m_Queue.Empty()) {
            const GEvent& Event = m_Queue.Peek_Front();
            Worker(Event);
            m_Queue.Pop();
        }
    }

    if (m_Simulator_Window) {
        if (!m_Recording) {
            m_Simulator_Thread.join();
        }
    }

    GInfo() << "GApp exiting Run() loop with _Ret = " << _Ret;
    return _Ret;
}


void GApplication::Simulator_Thread() {
    auto& File = m_Simulator_File;

    File.seekg(0, File.end);
    std::streamsize Count = File.tellg() / sizeof(GEvent);
    File.seekg(0, File.beg);

    GEvent* Event = new GEvent[Count];
    File.read((char*)Event, Count * sizeof(GEvent));

    for (std::streamsize i = 0; i < Count; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        m_Simulator_Window->Post_Event(Event[i]);
    }

    File.close();
}


// 'i' for Input
void GApplication::Resolve_Event(const GEvent& Event, std::ostream* i_Stream, const GString& Prefix, bool New_Line, bool Print_Active) {
    if (!Print_Active && Event.Type == GEType::Mouse && Event.Mouse_Message == GEMouse_Message::Active) return;

    std::ostream& Stream = *i_Stream;
    Stream << Prefix;

    switch (Event.Type) {
        case GEType::Window:
        {
            Stream<< "Window: ";

            switch (Event.Wind_Message) {
                case GEWind_Message::Resize:
                    Stream << "Resize(" << Event.WS << ")";
                    break;

                case GEWind_Message::Lose_Focus:
                    Stream << "Lose focus";
                    break;

                case GEWind_Message::Gain_Focus:
                    Stream << "Gain focus";
                    break;

                case GEWind_Message::Run:
                    Stream << "Run";
                    break;

                case GEWind_Message::Move:
                    Stream << "Move(" << Event.WP << ")";
                    break;

                case GEWind_Message::Render:
                    Stream << "Render";
                    break;

                case GEWind_Message::Show:
                    Stream << "Show";
                    break;

                case GEWind_Message::Hide:
                    Stream << "Hide";
                    break;

                case GEWind_Message::Close:
                    Stream << "Close";
                    break;

                case GEWind_Message::Iconify:
                    Stream << "Iconify";
                    break;

                case GEWind_Message::Should_Iconify:
                    Stream << "Should_Iconify";
                    break;

                case GEWind_Message::Maximise:
                    Stream << "Maximise";
                    break;

                case GEWind_Message::Should_Maximise:
                    Stream << "Should_Maximise";
                    break;

                case GEWind_Message::Restore:
                    Stream << "Restore";
                    break;

                case GEWind_Message::Should_Restore:
                    Stream << "Should_Restore";
                    break;

                case GEWind_Message::Terminate_Thread:
                    Stream << "Terminating thread";
                    break;

                default:
                    Stream << "--Unknown";
            }

            break;
        }

        case GEType::Mouse:
        {
            Stream << "Mouse: ";

            switch (Event.Mouse_Message) {
                case GEMouse_Message::Down:
                    Stream << "Down(" << Event.MP << ") - ";
                    switch (Event.Mouse_Button) {
                        case GEMouse_Button::LMB: Stream << "LMB"; break;
                        case GEMouse_Button::RMB: Stream << "RMB"; break;
                        case GEMouse_Button::MMB: Stream << "MMB"; break;
                    }

                    if (Event.Modifier_Shift) Stream << " (Shift)";
                    if (Event.Modifier_Alt) Stream << " (Alt)";
                    if (Event.Modifier_Ctrl) Stream << " (Ctrl)";

                    break;

                case GEMouse_Message::Up:
                    Stream << "Up(" << Event.MP << ") - ";
                    switch (Event.Mouse_Button) {
                        case GEMouse_Button::LMB: Stream << "LMB"; break;
                        case GEMouse_Button::RMB: Stream << "RMB"; break;
                        case GEMouse_Button::MMB: Stream << "MMB"; break;
                    }

                    if (Event.Modifier_Shift) Stream << " (Shift)";
                    if (Event.Modifier_Alt) Stream << " (Alt)";
                    if (Event.Modifier_Ctrl) Stream << " (Ctrl)";

                    break;

                case GEMouse_Message::Scroll_Up:
                    Stream << "Scroll_Up(" << Event.MP << ")";
                    break;

                case GEMouse_Message::Scroll_Down:
                    Stream << "Scroll_Down(" << Event.MP << ")";
                    break;

                case GEMouse_Message::Scroll_Left:
                    Stream << "Scroll_Left(" << Event.MP << ")";
                    break;

                case GEMouse_Message::Scroll_Right:
                    Stream << "Scroll_Right(" << Event.MP << ")";
                    break;

                case GEMouse_Message::Move:
                    Stream << "Move(" << Event.MP << ")";
                    break;

                case GEMouse_Message::Enter:
                    Stream << "Enter(" << Event.MP << ")";
                    break;

                case GEMouse_Message::Leave:
                    Stream << "Leave(" << Event.MP << ")";
                    break;

                case GEMouse_Message::Active:
                    Stream << "Is active?";
                    break;

                default:
                    Stream << "--Unknown";
            }

            break;
        }

        case GEType::Keyboard:
        {
            Stream << "Keyboard: ";

            switch (Event.Keyboard_Message) {
                case GEKeyboard_Message::Key:
                    Stream << "Key(key:" << Event.Key << ", scan_code:" << Event.Scancode << ") - ";
                    switch (Event.Key_Action) {
                        case GEKey_Action::Up: Stream << "Up"; break;
                        case GEKey_Action::Down: Stream << "Down"; break;
                        case GEKey_Action::Repeat: Stream << "Repeat"; break;
                    }

                    if (Event.Modifier_Shift) Stream << " (Shift)";
                    if (Event.Modifier_Alt) Stream << " (Alt)";
                    if (Event.Modifier_Ctrl) Stream << " (Ctrl)";

                    break;

                case GEKeyboard_Message::Text:
                    Stream << "Text(num:" << Event.Code_Point << ", ch: " << GString().push_back(Event.Code_Point) << ")";
                    break;

                default:
                    Stream << "--Unknown";
            }

            break;
        }
    }

    if (New_Line) Stream << std::endl;
}
