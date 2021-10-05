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

    m_Arrow_Cur       = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    m_IBeam_Cur       = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    m_Crosshair_Cur   = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    m_Hand_Cur        = glfwCreateStandardCursor(GLFW_POINTING_HAND_CURSOR);
    m_Resize_EW_Cur   = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR);
    m_Resize_NS_Cur   = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR);
    m_Resize_NWSE_Cur = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR);
    m_Resize_NESW_Cur = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR);
    m_Resize_All_Cur  = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR);
    m_Not_Allowed_Cur = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
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


void GApplication::Attach_Callbacks(GWindow* Window) {
    if (Window == m_Simulator_Window && !m_Recording) return;

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
    glfwSetWindowRefreshCallback(Window->m_Window_Hndl, GLFW_Window_Refresh_Callback);
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
        m_SEEvent = Event;
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

        case GECore_Message::Resize:
        {
            GWindow* Window = static_cast<GWindow*>(Event.Data_Ptr);
            glfwSetWindowSize(Window->m_Window_Hndl, Event.WS.X, Event.WS.Y);
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

        case GECore_Message::Set_Cursor:
        {
            GLFWcursor* Cursor = nullptr;
            switch (Event.Cursor_Type) {
                case GCursor_Type::Default:     Cursor = nullptr; break;
                case GCursor_Type::Arrow:       Cursor = m_Arrow_Cur; break;
                case GCursor_Type::IBeam:       Cursor = m_IBeam_Cur; break;
                case GCursor_Type::Crosshair:   Cursor = m_Crosshair_Cur; break;
                case GCursor_Type::Hand:        Cursor = m_Hand_Cur; break;
                case GCursor_Type::Resize_EW:   Cursor = m_Resize_EW_Cur; break;
                case GCursor_Type::Resize_NS:   Cursor = m_Resize_NS_Cur; break;
                case GCursor_Type::Resize_NWSE: Cursor = m_Resize_NWSE_Cur; break;
                case GCursor_Type::Resize_NESW: Cursor = m_Resize_NESW_Cur; break;
                case GCursor_Type::Resize_All:  Cursor = m_Resize_All_Cur; break;
                case GCursor_Type::Not_Allowed: Cursor = m_Not_Allowed_Cur; break;
                case GCursor_Type::Custom:      Cursor = ((GCursor*)Event.Data)->m_Ptr; break;

                default:
                    GWarning() << "Unknow (default) cursor type provided. Val:" << (int)Event.Cursor_Type;
            }

            GWindow* Window = static_cast<GWindow*>(Event.Data_Ptr);
            glfwSetCursor(Window->m_Window_Hndl, Cursor);
            break;
        }

        case GECore_Message::Create_Cursor:
        {
            GCursor& Cursor = *((GCursor*)Event.Data);

            GLFWimage Image;
            Image.width = Cursor.Size.X;
            Image.height = Cursor.Size.Y;
            Image.pixels = (unsigned char*)Cursor.Data;

            Cursor.m_Ptr = glfwCreateCursor(&Image, Cursor.Hot_Spot.X, Cursor.Size.Y - Cursor.Hot_Spot.Y);
            break;
        }

        case GECore_Message::Destroy_Cursor:
        {
            glfwDestroyCursor(((GCursor*)Event.Data)->m_Ptr);
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
            Worker(m_SEEvent);
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
        std::this_thread::sleep_for(std::chrono::milliseconds(70));

        if (Event[i].Type == GEType::Window) {
            if (Event[i].Wind_Message != GEWind_Message::Close &&
                Event[i].Wind_Message != GEWind_Message::Terminate_Thread &&
                Event[i].Wind_Message != GEWind_Message::Run &&
                Event[i].Wind_Message != GEWind_Message::Render) {
                Event[i].Data_Ptr = m_Simulator_Window;
            }
        }

        GApp()->Resolve_Event(Event[i], &std::cout);
        m_Simulator_Window->Post_Event(Event[i]);
    }

    File.close();
}


void GApplication::Resolve_Event(const GEvent& Event, std::ostream* Stream_Ptr, const GString& Prefix, bool New_Line, bool Print_Active, bool Is_Core) {
    if (!Print_Active && Event.Type == GEType::Mouse && Event.Mouse_Message == GEMouse_Message::Active) return;

    std::ostream& Stream = *Stream_Ptr;
    Stream << Prefix;

    if (Is_Core) {
        Stream << "Core: ";

        switch (Event.Core_Message) {
            case GECore_Message::Register:
                Stream << "Register(" << Event.Data_Ptr << ")";
                break;
            case GECore_Message::Run_Lambda:
                Stream << "Run_Lambda";
                break;
            case GECore_Message::Move:
                Stream << "Move(" << Event.WP << ")";
                break;
            case GECore_Message::Resize:
                Stream << "Resize(" << Event.WS << ")";
                break;
            case GECore_Message::Iconify:
                Stream << "Iconify(" << Event.Data_Ptr << ")";
                break;
            case GECore_Message::Maximise:
                Stream << "Maximise(" << Event.Data_Ptr << ")";
                break;
            case GECore_Message::Restore:
                Stream << "Restore(" << Event.Data_Ptr << ")";
                break;
            case GECore_Message::Show:
                Stream << "Show(" << Event.Data_Ptr << ")";
                break;
            case GECore_Message::Hide:
                Stream << "Hide(" << Event.Data_Ptr << ")";
                break;
            case GECore_Message::Close:
                Stream << "Close(" << Event.Data_Ptr << ")";
                break;
            case GECore_Message::Terminate:
                Stream << "Terminate";
                break;

            default:
                Stream << "Core -- Unknown";
        }

        if (New_Line) Stream << std::endl;
        return;
    }


    switch (Event.Type) {
        case GEType::Window:
        {
            Stream << "Window: ";

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
                    Stream << "Window -- Unknown";
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

                case GEMouse_Message::Gain_Focus:
                    Stream << "Gain mouse focus (send mouse msgs to that window)";
                    break;

                case GEMouse_Message::Lose_Focus:
                    Stream << "Lose mouse focus";
                    break;

                default:
                    Stream << "Mouse -- Unknown";
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
                    Stream << "Keyboard -- Unknown";
            }

            break;
        }
    }

    if (New_Line) Stream << std::endl;
}
