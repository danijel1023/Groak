#pragma once
#include "GCore.h"
#include "GQuad.h"
#include <vector>

class GWindow;


class GBasic_Window {
public:
    GBasic_Window(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y);
    virtual ~GBasic_Window();
    
    void Add_Framebuffer();
    void Add_Quad(GQuad& Quad);

protected:
    int m_Screen_X = 0, m_Screen_Y = 0, m_Absolute_Pos_X = 0, m_Absolute_Pos_Y = 0;
    int m_Window_X = 0, m_Window_Y = 0;
    GWindow* m_Main_Window = nullptr;
    GBasic_Window* m_Parent = nullptr;

    std::vector<GBasic_Window*> m_Child_Windows;
    std::vector<GQuad> m_Quad_List;

    GDispatcher m_Dispatcher_Ptr = &GBasic_Window::Dispatcher_Func;
    int Dispatcher_Func(GEvent* Event);

    GCallback m_Callback_Ptr = &GBasic_Window::Callback_Func;
    int Callback_Func(GEvent* Event);

private:
    static int Dispatcher_Func(void* _This, GEvent* Event);
    static int Callback_Func(void* _This, GEvent* Event);

public:
    //"Getters" and "setters"
    inline int Get_Screen_X() { return m_Screen_X; }
    inline int Get_Screen_Y() { return m_Screen_Y; }
    inline int Get_Window_X() { return m_Window_X; }
    inline int Get_Window_Y() { return m_Window_Y; }
    virtual void Set_Screen_X(int Value) { m_Screen_X; }
    virtual void Set_Screen_Y(int Value) { m_Screen_Y; }
    virtual void Set_Window_X(int Value) { m_Window_X; }
    virtual void Set_Window_Y(int Value) { m_Window_Y; }

    inline GWindow* Main_Window() { return m_Main_Window; }

    friend class GWindow;
};