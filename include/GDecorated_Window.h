#pragma once
#include "GWindow.h"
#include "GTitle_Bar.h"
#include "GResize_Area.h"


class GDecorated_Window : public GWindow {
public:
    GDecorated_Window(const GString& Name, int Window_X, int Window_Y);
    GDecorated_Window(const GString& Name, const GSize& Window);

    ~GDecorated_Window();

    void Set_Min_Size(const GSize& Size);
    void Set_Max_Size(const GSize& Size);
    GSize Get_Min_Size();
    GSize Get_Max_Size();

private:
    GTitle_Bar* m_Title_Bar = nullptr;
    GResize_Area* m_Top_Right = nullptr, * m_Top = nullptr, * m_Top_Left = nullptr;
    GResize_Area* m_Left = nullptr;
    GResize_Area* m_Bot_Left = nullptr, * m_Bot = nullptr, * m_Bot_Right = nullptr;
    GResize_Area* m_Right = nullptr;

    GSize m_Min_Size, m_Max_Size;

protected:
    int Callback_Func(const GEvent& Event);

private:
    static int Callback_Func(void* _This, const GEvent& Event) {
        auto This = static_cast<GDecorated_Window*>(_This);
        return This->Callback_Func(Event);
    }
};
