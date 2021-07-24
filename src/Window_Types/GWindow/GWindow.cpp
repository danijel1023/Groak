#include "GWindow.h"
#include "GApplication.h"
#include "GLog_Manager.h"

GWindow::GWindow(const GString& Name, int Window_X, int Window_Y, int Screen_X, int Screen_Y)
    : GBasic_Window(nullptr, Window_X, Window_Y, Screen_X, Screen_Y), m_Name(Name) {
    m_Main_Window = this;
    m_Dispatcher_Ptr = &GWindow::Dispatcher_Func;
    m_Callback_Ptr = &GWindow::Callback_Func;
}

GWindow::~GWindow() {}



int GWindow::Send_Event(const GEvent& Event) {
    std::unique_lock<std::recursive_mutex> Lck(m_Dispatcher_Mutex);
    return GCall(this, m_Dispatcher_Ptr, &Event);
}

void GWindow::Post_Event(const GEvent& Event) {
    auto Node = m_Queue.Get_Node();
    Node->Data = Event;
    m_Queue.Insert(Node);

    m_DCV.notify_all();
}


void GWindow::Set_Focus(GBasic_Window* Window) {
    m_Focus = Window;
}



int GWindow::Callback_Func(void* _This, const GEvent* Event) {
    auto This = static_cast<GWindow*>(_This);
    return This->Callback_Func(Event);
}

int GWindow::Dispatcher_Func(void* _This, const GEvent* Event) {
    auto This = static_cast<GWindow*>(_This);
    return This->Dispatcher_Func(Event);
}


int GWindow::Dispatcher_Func(const GEvent* Event) {
    switch (Event->Type) {
        case GEType::Custom:
        {
            auto Direct_This = static_cast<GBasic_Window*>(Event->Data_Ptr);
            return GCall(Direct_This, m_Callback_Ptr, Event);
        }

        case GEType::Keyboard:
        {
            return GCall(m_Focus, m_Callback_Ptr, Event);
        }

        case GEType::Mouse:
        {
            if (Event->Mouse_Message == GEMouse_Message::Move) {
                if (m_Mouse_Focus) {
                    return GCall(m_Mouse_Focus, m_Callback_Ptr, Event);
                }
            }

            else if (Event->Mouse_Message == GEMouse_Message::Leave) {
                if (m_Wind_Under_Cursor) GCall(m_Wind_Under_Cursor, m_Callback_Ptr, Event);
                m_Wind_Under_Cursor = nullptr;
                return 1;
            }

            break;
        }

        case GEType::Window:
        {
            if (Event->Wind_Message == GEWind_Message::Move) {
                return GCall(this, m_Callback_Ptr, Event);
            }

            break;
        }
    }

    return GBasic_Window::Dispatcher_Func(Event);
}

int GWindow::Callback_Func(const GEvent* Event) {
    if (Event->Type == GEType::Window) {
        switch (Event->Wind_Message) {
            case GEWind_Message::Gain_Focus:
            {
                m_Focused = true;
                break;
            }

            case GEWind_Message::Lose_Focus:
            {
                m_Focused = false;
                break;
            }

            case GEWind_Message::Move:
            {
                m_Screen_X = Event->WP.X;
                m_Screen_Y = Event->WP.Y;
                return 0;
            }
        }
    }

    return GBasic_Window::Callback_Func(Event);
}



void GWindow::Run() {
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    m_Window_Hndl = glfwCreateWindow(m_Window_X, m_Window_Y, m_Name.c_str(), NULL, NULL);
    if (!m_Window_Hndl) {
        const char* Error_Msg;
        glfwGetError(&Error_Msg);
        GError() << "Could not create window: " << m_Name;
        GError() << Error_Msg;
        return;
    }

    GApp()->Set_Context(this);

    if (!gladLoadGL()) {
        GError() << "GLAD: Failed to initialize OpenGL context";
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetWindowPos(m_Window_Hndl, m_Screen_X, m_Screen_Y);

    GApp()->Attach_Callbacks(this);

    glClearColor((86.0f / 255.0f), (156.0f / 255.0f), (214.0f / 255.0f), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_Window_Hndl);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_Window_Hndl);

    m_Renderer = new GRenderer(this);
    m_Renderer->Set_Window_Screen(0, 0, m_Window_X, m_Window_Y);

    m_Worker = std::thread(&GWindow::Worker, this);

    GEvent Run_E;
    Run_E.Type = GEType::Window;
    Run_E.Wind_Message = GEWind_Message::Run;
    Send_Event(Run_E);
}

void GWindow::Worker() {
    while (m_Running) {
        std::unique_lock<std::recursive_mutex> Lck(m_Dispatcher_Mutex);
        m_DCV.wait(Lck, [=] { return !m_Queue.Empty(); });

        while (!m_Queue.Empty()) {
            GEvent Event = m_Queue.Peek_Front();
            GCall(this, m_Dispatcher_Ptr, &Event);
            m_Queue.Pop();
        }
    }
}


void GWindow::Terminate() {
    m_Running = false;

    GEvent Event;
    Event.Type = GEType::Window;
    Event.Wind_Message = GEWind_Message::Terminate_Thread;

    Post_Event(Event);
    m_Worker.join();
}
