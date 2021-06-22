#include "GApplication.h"
#include "GWindow.h"

void GApplication::GLFW_Key_Callback(GLFWwindow* Window_Hndl, int Key, int Scancode, int Action, int Mods) {
    auto _This = static_cast<GWindow*>(glfwGetWindowUserPointer(Window_Hndl));

    GEvent Event;
    Event.Type = GEType::Keyboard;
    Event.Keyboard_Message = GEKeyboard_Message::Key;

    switch (Action) {
        case GLFW_PRESS:    Event.Key_Action = GEKey_Action::Down;      break;
        case GLFW_RELEASE:  Event.Key_Action = GEKey_Action::Up;        break;
        case GLFW_REPEAT:   Event.Key_Action = GEKey_Action::Repeat;    break;
        default: return;    //Not supported other buttons
    }

    if (Mods & GLFW_MOD_SHIFT)      Event.Modifier_Shift = true;
    if (Mods & GLFW_MOD_ALT)        Event.Modifier_Alt = true;
    if (Mods & GLFW_MOD_CONTROL)    Event.Modifier_Ctrl = true;

    Event.Key = Key;
    _This->Post_Event(Event);
}

void GApplication::GLFW_Char_Callback(GLFWwindow* Window_Hndl, unsigned int Code_Point) {
    auto _This = static_cast<GWindow*>(glfwGetWindowUserPointer(Window_Hndl));

    GEvent Event;
    Event.Type = GEType::Keyboard;
    Event.Keyboard_Message = GEKeyboard_Message::Text;
    Event.Code_Point = Code_Point;

    _This->Post_Event(Event);
}



void GApplication::GLFW_Cursor_Pos_Callback(GLFWwindow* Window_Hndl, double X_Pos, double Y_Pos) {
    auto _This = static_cast<GWindow*>(glfwGetWindowUserPointer(Window_Hndl));

    GEvent Event;
    Event.Type = GEType::Mouse;
    Event.Mouse_Message = GEMouse_Message::Move;
    Event.MP = { (int)X_Pos, _This->m_Window_Y - (int)Y_Pos};

    _This->Post_Event(Event);
}

void GApplication::GLFW_Cursor_Enter_Callback(GLFWwindow* Window_Hndl, int Entered) {
    if (!Entered) {
        auto _This = static_cast<GWindow*>(glfwGetWindowUserPointer(Window_Hndl));

        GEvent Event;
        Event.Type = GEType::Mouse;
        Event.Mouse_Message = GEMouse_Message::Leave;
        
        double X, Y;
        glfwGetCursorPos(Window_Hndl, &X, &Y);
        Event.MP = { (int)X, _This->m_Window_Y - (int)Y };
        _This->Post_Event(Event);
    }
}


void GApplication::GLFW_Mouse_Button_Callback(GLFWwindow* Window_Hndl, int Button, int Action, int Mods) {
    auto _This = static_cast<GWindow*>(glfwGetWindowUserPointer(Window_Hndl));

    GEvent Event;
    Event.Type = GEType::Mouse;

    switch (Button) {
        case GLFW_MOUSE_BUTTON_LEFT:    Event.Mouse_Button = GEMouse_Button::LMB;   break;
        case GLFW_MOUSE_BUTTON_RIGHT:   Event.Mouse_Button = GEMouse_Button::RMB;   break;
        case GLFW_MOUSE_BUTTON_MIDDLE:  Event.Mouse_Button = GEMouse_Button::MMB;   break;
        default: return;    //Not supported other buttons
    }

    if (Action == GLFW_PRESS)   Event.Mouse_Message = GEMouse_Message::Down;
    else                        Event.Mouse_Message = GEMouse_Message::Up;

    if (Mods & GLFW_MOD_SHIFT)      Event.Modifier_Shift = true;
    if (Mods & GLFW_MOD_ALT)        Event.Modifier_Alt = true;
    if (Mods & GLFW_MOD_CONTROL)    Event.Modifier_Ctrl = true;

    double X, Y;
    glfwGetCursorPos(Window_Hndl, &X, &Y);
    Event.MP = { (int)X, _This->m_Window_Y - (int)Y };
    _This->Post_Event(Event);
}

void GApplication::GLFW_Scroll_Callback(GLFWwindow* Window_Hndl, double X_Offset, double Y_Offset) {
    auto _This = static_cast<GWindow*>(glfwGetWindowUserPointer(Window_Hndl));

    GEvent Event;
    Event.Type = GEType::Mouse;

    if (Y_Offset == -1) Event.Mouse_Message = GEMouse_Message::Scroll_Down;
    else if (Y_Offset == 1) Event.Mouse_Message = GEMouse_Message::Scroll_Up;
    
    if (X_Offset == -1) Event.Mouse_Message = GEMouse_Message::Scroll_Left;
    else if (X_Offset == 1) Event.Mouse_Message = GEMouse_Message::Scroll_Right;

    double X, Y;
    glfwGetCursorPos(Window_Hndl, &X, &Y);
    Event.MP = { (int)X, _This->m_Window_Y - (int)Y };
    _This->Post_Event(Event);
}



void GApplication::GLFW_Window_Size_Callback(GLFWwindow* Window_Hndl, int Width, int Height) {
    auto _This = static_cast<GWindow*>(glfwGetWindowUserPointer(Window_Hndl));

    GEvent Event;
    Event.Type = GEType::Window;
    Event.Wind_Message = GEWind_Message::Resize;
    Event.WS = { Width, Height };

    _This->Post_Event(Event);
}

void GApplication::GLFW_Window_Pos_Callback(GLFWwindow* Window_Hndl, int X_Pos, int Y_Pos) {
    auto _This = static_cast<GWindow*>(glfwGetWindowUserPointer(Window_Hndl));

    GEvent Event;
    Event.Type = GEType::Window;
    Event.Wind_Message = GEWind_Message::Move;
    Event.WP = { X_Pos, Y_Pos};

    _This->Post_Event(Event);
}

void GApplication::GLFW_Window_Focus_Callback(GLFWwindow* Window_Hndl, int Focused) {
    auto _This = static_cast<GWindow*>(glfwGetWindowUserPointer(Window_Hndl));

    GEvent Event;
    Event.Type = GEType::Window;

    if (Focused) Event.Wind_Message = GEWind_Message::Lose_Focus;
    else Event.Wind_Message = GEWind_Message::Gain_Focus;

    _This->Post_Event(Event);
}