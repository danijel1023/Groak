#pragma once
#include "GCore.h"
#include "GQuad.h"
#include "GFramebuffer.h"
#include <vector>


class GWindow;
class GBasic_Window {
public:
    GBasic_Window(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y, bool Overlay = false);
    GBasic_Window(GBasic_Window* Parent, const GSize& Window, const GPos& Screen, bool Overlay = false);
    virtual ~GBasic_Window();
    
    GFramebuffer* Create_Framebuffer();
    size_t Add_Quad(const GQuad& Quad);
    GQuad& Get_Quad(size_t Quad_i);
    void Set_Viewport();
    void Render();

    bool m_Enabled = true;

protected:
    GPos  m_Screen, m_Absolute_Screen;
    GSize m_Window;
    GWindow* m_Main_Window = nullptr;
    GBasic_Window* m_Parent = nullptr;

    std::vector<GBasic_Window*> m_Child_Windows;
    std::vector<GBasic_Window*> m_Overlay_Windows;
    std::vector<GQuad> m_Quad_List;

    GDispatcher m_Dispatcher_Ptr = &GBasic_Window::Dispatcher_Func;
    int Dispatcher_Func(const GEvent& Event);

    GCallback m_Callback_Ptr = &GBasic_Window::Callback_Func;
    int Callback_Func(const GEvent& Event);

private:
    static int Dispatcher_Func(void* _This, const GEvent& Event);
    static int Callback_Func(void* _This, const GEvent& Event);

public:
    //"Getters" and "setters"
    inline const GPos  Get_Absolute_Screen() { return m_Absolute_Screen; }
    inline const GPos  Get_Screen() { return m_Screen; }
    inline const GSize Get_Window() { return m_Window; }
    void Set_Screen(const GPos& Pos);
    void Set_Window(const GSize& Size);

    inline GWindow* Main_Window() { return m_Main_Window; }

    friend class GWindow;
    friend class GFramebuffer;
    friend class GRenderer;
};
