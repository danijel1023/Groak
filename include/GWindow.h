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

    int Send_Event(const GEvent& Event);
    void Post_Event(const GEvent& Event);

    void Set_Focus(GBasic_Window* Window);

    GString Get_Name() { return m_Name; }
    GRenderer* Get_Renderer() { return m_Renderer; }
    unsigned int Get_Last_FB() {
        if (m_Last_Framebuffer.top()) return m_Last_Framebuffer.top()->Get_FB();
        else                          return 0;
    }

protected:
    int Dispatcher_Func(const GEvent* Event);
    int Callback_Func(const GEvent* Event);

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
    std::stack<GFramebuffer*> m_Last_Framebuffer = std::stack<GFramebuffer*>({ nullptr });
    GPos m_FB_Ancor = { 0, 0 };

    static int Dispatcher_Func(void* _This, const GEvent* Event);
    static int Callback_Func(void* _This, const GEvent* Event);

    friend class GApplication;
    friend class GBasic_Window;
    friend class GFramebuffer;
};
