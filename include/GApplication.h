#pragma once
#include <vector>
#include <condition_variable>
#include <fstream>
#include <thread>

#include "GCore.h"


class GWindow;
class GApplication {
public:
    GApplication();
    ~GApplication();
    int Run();

    static GApplication* m_Instance;

    void Post_Event(const GEvent& Event);
    void Send_Event_NL(const GEvent& Event);
    void Send_Event(const GEvent& Event);

    void Register_Window(GWindow* Window);
    void Set_Context(GWindow* Window);

    void Attach_Simulator(GWindow* Window, bool Recording);
    void Attach_Callbacks(GWindow* Window);

private:
    bool m_Running = true;
    void Worker(const GEvent& Event);
    GWindow* Current_Context = nullptr;

    std::recursive_mutex m_SERM;
    std::condition_variable_any m_SECV;
    bool m_SE = false;
    bool m_SE_Continue = false;
    const GEvent* m_SEEvent = nullptr;

    std::recursive_mutex m_QRM;
    GTEQueue m_Queue;

    std::vector<GWindow*> m_Window_List;

    GWindow* m_Simulator_Window = nullptr;
    bool m_Recording = false;
    std::fstream m_Simulator_File;
    std::thread m_Simulator_Thread;
    void Simulator_Thread();



    // Callbacks
    static void GLFW_Key_Callback(GLFWwindow* Window_Hndl, int Key, int Scancode, int Action, int Mods);
    static void GLFW_Char_Callback(GLFWwindow* Window_Hndl, unsigned int Code_Point);

    static void GLFW_Cursor_Pos_Callback(GLFWwindow* Window_Hndl, double X_Pos, double Y_Pos);
    static void GLFW_Cursor_Enter_Callback(GLFWwindow* Window_Hndl, int Entered);
    static void GLFW_Mouse_Button_Callback(GLFWwindow* Window_Hndl, int Button, int Action, int Mods);
    static void GLFW_Scroll_Callback(GLFWwindow* Window_Hndl, double X_Offset, double Y_Offset);

    static void GLFW_Window_Size_Callback(GLFWwindow* Window_Hndl, int Width, int Height);
    static void GLFW_Window_Pos_Callback(GLFWwindow* Window_Hndl, int X_Pos, int Y_Pos);
    static void GLFW_Window_Focus_Callback(GLFWwindow* Window_Hndl, int Focused);
    static void GLFW_Window_Close_Callback(GLFWwindow* Window_Hndl);
    static void GLFW_Window_Maximize_Callback(GLFWwindow* Window_Hndl, int Maximized);
    static void GLFW_Window_Iconify_Callback(GLFWwindow* Window_Hndl, int Iconified);

    // Callbacks with recording
    static void GLFW_Recording_Key_Callback(GLFWwindow* Window_Hndl, int Key, int Scancode, int Action, int Mods);
    static void GLFW_Recording_Char_Callback(GLFWwindow* Window_Hndl, unsigned int Code_Point);

    static void GLFW_Recording_Cursor_Pos_Callback(GLFWwindow* Window_Hndl, double X_Pos, double Y_Pos);
    static void GLFW_Recording_Cursor_Enter_Callback(GLFWwindow* Window_Hndl, int Entered);
    static void GLFW_Recording_Mouse_Button_Callback(GLFWwindow* Window_Hndl, int Button, int Action, int Mods);
    static void GLFW_Recording_Scroll_Callback(GLFWwindow* Window_Hndl, double X_Offset, double Y_Offset);

    static void GLFW_Recording_Window_Size_Callback(GLFWwindow* Window_Hndl, int Width, int Height);
    static void GLFW_Recording_Window_Pos_Callback(GLFWwindow* Window_Hndl, int X_Pos, int Y_Pos);
    static void GLFW_Recording_Window_Focus_Callback(GLFWwindow* Window_Hndl, int Focused);
    static void GLFW_Recording_Window_Close_Callback(GLFWwindow* Window_Hndl);
    static void GLFW_Recording_Window_Maximize_Callback(GLFWwindow* Window_Hndl, int Maximized);
    static void GLFW_Recording_Window_Iconify_Callback(GLFWwindow* Window_Hndl, int Iconified);
};
