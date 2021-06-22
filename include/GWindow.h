#pragma once
#include <mutex>
#include <condition_variable>
#include <thread>

#include "GCore.h"
#include "GBasic_Window.h"
#include "GRenderer.h"


class GWindow : public GBasic_Window {
public:
    GWindow(const GString& Name, int Window_X, int Window_Y, int Screen_X = 0, int Screen_Y = 0);
    virtual ~GWindow();

    int Send_Event(GEvent& Event);
    void Post_Event(GEvent& Event);

    GString Get_Name();
    void Set_Focus(GBasic_Window* Window);

protected:
    int Dispatcher_Func(GEvent* Event);
    int Callback_Func(GEvent* Event);

    bool m_Focused = true;
    GBasic_Window* m_Focus = this;

private:
    GLFWwindow* m_Window_Hndl = nullptr;
    GString m_Name;
    bool m_Running = true;
    GBasic_Window* m_Wind_Under_Cursor = nullptr;
    GBasic_Window* m_Mouse_Focus = nullptr;

    GTEQueue m_Queue;
    std::recursive_mutex m_Dispatcher_Mutex;
    std::condition_variable_any m_DCV;

    void Terminate();

    void Run();
    std::thread m_Worker;
    void Worker();

    GRenderer* m_Renderer = nullptr;
    std::vector<unsigned int> m_Last_Framebuffer;

    static int Dispatcher_Func(void* _This, GEvent* Event);
    static int Callback_Func(void* _This, GEvent* Event);

    friend class GApplication;
    friend class GBasic_Window;
};
