#pragma once
#include <stdint.h>
#include <functional>
#include "GUnits.h"

//Core type doesn't exist since you have
//to use GApp-calls (eg. GApp()->Post_Event(...)) directly, to write to its queue
enum class GEType { Window, Mouse, Keyboard, Custom };

enum class GECore_Message {
    Register, Run_Lambda,

    Move, Resize,
    Iconify, Maximise, Restore,
    Show, Hide,
    Close,

    Terminate
};

enum class GEWind_Message {
    Resize, Lose_Focus, Gain_Focus,

    Run,
    Move, Iconify, Maximise, Restore,
    Should_Iconify, Should_Maximise, Should_Restore,
    Render, Show, Hide,
    Close, Terminate_Thread
};


enum class GEMouse_Button { RMB, LMB, MMB };
enum class GEMouse_Message {
    Down, Up,

    Scroll_Up, Scroll_Down,
    Scroll_Left, Scroll_Right,

    Move, Enter, Leave, Active,
    Gain_Focus, Lose_Focus
};


enum class GEKeyboard_Message { Key, Text };
enum class GEKey_Action { Up, Down, Repeat };


struct GEvent {
    GEType Type = {};
    union {
        GECore_Message Core_Message = {};
        GEWind_Message Wind_Message;
        GEMouse_Message Mouse_Message;
        GEKeyboard_Message Keyboard_Message;
    };

    int64_t Data = 0;
    void* Data_Ptr = 0;

    GSize WS;   //Window size
    GPos  WP;   //Window position
    GPos MP;    //Mouse position

    GEMouse_Button Mouse_Button = {};

    std::function<int()> Lambda;

    struct {
        bool Modifier_Alt = false;
        bool Modifier_Shift = false;
        bool Modifier_Ctrl = false;
    };


    int Key = 0;
    int Scancode = 0;
    uint32_t Code_Point = 0;
    GEKey_Action Key_Action = {};
};
