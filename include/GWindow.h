#pragma once
#include <mutex>
#include <condition_variable>
#include <thread>

#include "GCore.h"
#include "GBasic_Window.h"
#include "GRenderer.h"


class GWindow : public GBasic_Window {
public:
    GWindow(const GString& Name, int Window_X, int Window_Y);
    GWindow(const GString& Name, const GSize& Window);
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

    GTexture Load_Texture(const GString& Path, bool Flip = false);
    GTexture Load_Texture_From_Memory(const unsigned char* Mem_Data, unsigned int Size, bool Flip = false);

    GFont* Load_Font(const GString& Font_File);
    GFont* Set_Default_Font(GFont* Font);
    GFont* Get_Default_Font();
    void Set_Text_Height(int Height);   //Use default font
    void Set_Text_Height(int Height, GFont* Font);

    void Render();

protected:
    int Dispatcher_Func(const GEvent& Event);
    int Callback_Func(const GEvent& Event);

    bool m_Focused = true;
    GBasic_Window* m_Focus = this;

private:
    GLFWwindow* m_Window_Hndl = nullptr;
    GString m_Name;
    bool m_Running = true, m_Recording = false;
    GBasic_Window* m_Mouse_Focus = nullptr, * m_Wind_Under_Cursor = nullptr;
    int m_Mouse_Buttons_Pressed = 0;

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

    GFont* m_Default_Font = nullptr;
    std::vector<GFont*> m_Font_List;

    std::vector<GTexture> m_Texture_List;

    static int Dispatcher_Func(void* _This, const GEvent& Event);
    static int Callback_Func(void* _This, const GEvent& Event);

    friend class GApplication;
    friend class GBasic_Window;
    friend class GFramebuffer;
};
