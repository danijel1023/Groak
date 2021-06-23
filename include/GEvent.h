#include <stdint.h>
#include <functional>

//Core type doesn't exist since you have
//to use GApp-calls (eg. GApp()->Post_Event(...)) directly, to write to its queue
enum class GEType { Window, Mouse, Keyboard, Custom };

enum class GECore_Message {
    Register, Run_Lambda,

    Move, Minimise, Maximise, Recenter,
    Render, Show, Hide,
    Close,

    Terminate
};

enum class GEWind_Message {
    Resize, Lose_Focus, Gain_Focus,

    Run,
    Move, Minimise, Maximise, Recenter,
    Render, Show, Hide,
    Close, Terminate_Thread
};


enum class GEMouse_Button { RMB, LMB, MMB };
enum class GEMouse_Message {
    Down, Up,

    Scroll_Up, Scroll_Down,
    Scroll_Left, Scroll_Right,

    Move, Enter, Leave
};


enum class GEKeyboard_Message { Key, Text };
enum class GEKey_Action { Up, Down, Repeat };



struct GPos {
    int X = 0, Y = 0;

    GPos operator+(GPos Right) { return { (X + Right.X), (Y + Right.Y) }; }
    GPos operator-(GPos Right) { return { (X - Right.X), (Y - Right.Y) }; }

    GPos& operator+=(GPos Right) { X += Right.X; Y += Right.Y; return *this; }
    GPos& operator-=(GPos Right) { X -= Right.X; Y -= Right.Y; return *this; }
};

using GSize = GPos;


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
    GSize FS;   //Framebuffer size
    GPos  WP;   //Window position

    std::function<int()> Lambda;

    struct {
        bool Modifier_Alt = false;
        bool Modifier_Shift = false;
        bool Modifier_Ctrl = false;
    };

    GPos MP;    //Mouse position
    GEMouse_Button Mouse_Button = {};

    int Key = 0;
    uint32_t Code_Point = 0;
    GEKey_Action Key_Action = {};
};
