#pragma once
#include <stdint.h>
#include <functional>
#include "GUnits.h"
#include "GTexture.h"
#include "GString.h"

enum class GEType { Core, Window, Mouse, Keyboard, Renderer, Console, Custom };

enum class GECore_Message {
    Register, Run_Lambda,

    Create_Cursor, Set_Cursor, Destroy_Cursor,
    Get_Clipboard, Set_Clipboard,

    Move, Resize,
    Iconify, Maximise, Restore,
    Show, Hide,
    Close,

    Terminate
};

enum class GCursor_Type {
    Default, Arrow, IBeam,
    Crosshair, Hand, Resize_EW,
    Resize_NS, Resize_NWSE,
    Resize_NESW, Resize_All, Not_Allowed,
    Custom
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

enum class GERenderer_Message {
    Terminate_Thread,
    Render,
    Send_Event,
    Load_Texture
};

enum class GEConsole_Message {
    Sync
};


struct GEvent {
    GEType Type = {};
    union {
        GECore_Message Core_Message = {};
        GEWind_Message Wind_Message;
        GEMouse_Message Mouse_Message;
        GEKeyboard_Message Keyboard_Message;
        GERenderer_Message Renderer_Message;
        GEConsole_Message Console_Message;
    };

    int64_t Data = 0;
    void* Data_Ptr = 0;

    GSize WS;   //Window size
    GPos  WP;   //Window position

    GEMouse_Button Mouse_Button = {};
    GPos MP;    //Mouse position

    std::function<int()> Lambda;
    GTexture Texture;

    struct {
        bool Modifier_Alt = false;
        bool Modifier_Shift = false;
        bool Modifier_Ctrl = false;
    };

    GCursor_Type Cursor_Type = {};

    int Key = 0;
    int Scancode = 0;
    uint32_t Code_Point = 0;
    GEKey_Action Key_Action = {};


    void Resolve_Event(std::ostream* Stream_Ptr, const GString& Prefix = "", bool New_Line = true, bool Print_Active = false) const;
};
