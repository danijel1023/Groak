#include "GConsole/GConsole.h"
#include "GApplication.h"
#include "GLog_Manager.h"

static const unsigned char Cursor_Image[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0xCF, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xDE, 0x00, 0x00, 0x00, 0x24,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xE7,
    0x00, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0xEE, 0x00, 0x00, 0x00, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xF4, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x52,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFB, 0x00, 0x00, 0x00, 0xD2,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xF2,
    0x00, 0x00, 0x00, 0xF4, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x47,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x6B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0xD5, 0x00, 0x00, 0x00, 0xDB, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x67, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x52, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0xED, 0x00, 0x00, 0x00, 0xA7, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


GConsole::GConsole()
    : GDecorated_Window("Groak Console", 500, 250), m_Stream_Buff(this), m_Stream(&m_Stream_Buff) {
    m_Callback_Ptr = &GConsole::Callback_Func;
    Set_Min_Size({184, 32});
    Set_Max_Size({700, 350});

    //GApp()->Attach_Simulator(this, false);
}

GConsole::~GConsole() {
    GLog_Manager::Close_GConsole();
    GLog_Manager::Set_Device(GLog_Device::Std_Console);
}



void GConsole::Show() {
    GEvent Event;
    Event.Core_Message = GECore_Message::Show;
    Event.Data_Ptr = this;
    GApp()->Post_Event(Event);
}

void GConsole::Hide() {
    GEvent Event;
    Event.Core_Message = GECore_Message::Hide;
    Event.Data_Ptr = this;
    GApp()->Post_Event(Event);
}

std::ostream* GConsole::Get_Stream() { return &m_Stream; }

GPos MP_C;
bool Track_C = false;


static GText Text;
static int Text_Height = 20;

GCursor My_Cursor;
int GConsole::Callback_Func(const GEvent& Event) {
    //return GDecorated_Window::Callback_Func(Event);

    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Run:
                {
                    GQuad Quad({ 2, 100 }, { 50, 0 });
                    Quad.m_Color = { 1.0f, 0.0f, 0.0f, 1.0f };
                    Quad.m_Rotation = 31;

                    Add_Quad(Quad);
                    Render();

                    My_Cursor.Data = (const char*)Cursor_Image;
                    My_Cursor.Hot_Spot = {5, 13};
                    My_Cursor.Size = {16, 16};
                    
                    GEvent Event;
                    Event.Core_Message = GECore_Message::Create_Cursor;
                    Event.Data = (int64_t)&My_Cursor;
                    GApp()->Post_Event(Event);

                    //Set_Default_Font(Load_Font("C:/Windows/Fonts/consola.ttf"));
                    Set_Default_Font(Load_Font("C:/Windows/Fonts/calibri.ttf"));
                    //calibri.ttf

                    Text.push_back({ 'R', {0.94, 0.96, 0.25} });
                    Text.push_back({ 'e', {0.92, 0.25, 0.2} });
                    Text.push_back({ 'c', {1.0, 0.0, 1.0} });
                    Text.push_back({ 'o', {1.0, 0.0, 1.0} });
                    Text.push_back({ 'r', {1.0, 0.0, 1.0} });
                    Text.push_back({ 'd', {1.0, 0.0, 1.0} });
                    Text.push_back({ 'i', {1.0, 0.0, 1.0} });
                    Text.push_back({ 'n', {1.0, 0.0, 1.0} });
                    Text.push_back({ 'g', {1.0, 0.0, 1.0} });
                    Text.push_back({ '.', {1.0, 0.0, 1.0} });
                    Text.push_back({ '.', {1.0, 0.0, 1.0} });
                    Text.push_back({ '.', {1.0, 0.0, 1.0} });
                    //
                    break;
                }

                case GEWind_Message::Close:
                {
                    GEvent Destroy_Cur;
                    Destroy_Cur.Core_Message = GECore_Message::Destroy_Cursor;
                    Destroy_Cur.Data = (int64_t)&My_Cursor;
                    GApp()->Post_Event(Destroy_Cur);

                    GEvent Event;
                    Event.Core_Message = GECore_Message::Terminate;
                    GApp()->Post_Event(Event);
                    break;
                }

                case GEWind_Message::Render:
                {
                    GRenderer& Renderer = *Get_Renderer();
                    Renderer.Clear();

                    //Default
                    GWindow::Callback_Func(Event);

                    //Render text
                    Renderer.Draw_Text(Text, { 50, 50 }, Text_Height);
                    Renderer.Flush();
                    return 0;
                }
            }

            break;
        }

        case GEType::Keyboard:
        {
            if (Event.Keyboard_Message == GEKeyboard_Message::Key) {
                if (Event.Key_Action == GEKey_Action::Down && Event.Key == 256) {
                    GEvent Event;
                    Event.Core_Message = GECore_Message::Terminate;
                    GApp()->Post_Event(Event);
                }
            }

            break;
        }

        case GEType::Mouse:
        {
            switch (Event.Mouse_Message) {
                case GEMouse_Message::Enter:
                {
                    GEvent Event;
                    Event.Core_Message = GECore_Message::Set_Cursor;
                    Event.Data_Ptr = m_Main_Window;
                    Event.Cursor_Type = GCursor_Type::Custom;
                    Event.Data = (int64_t)&My_Cursor;
                    GApp()->Post_Event(Event);

                    break;
                }

                case GEMouse_Message::Leave:
                {
                    GEvent Event;
                    Event.Core_Message = GECore_Message::Set_Cursor;
                    Event.Data_Ptr = m_Main_Window;
                    Event.Cursor_Type = GCursor_Type::Default;
                    GApp()->Post_Event(Event);
                    break;
                }

                case GEMouse_Message::Scroll_Down:
                {
                    if (Text_Height) Text_Height--;
                    Render();
                    break;
                }

                case GEMouse_Message::Scroll_Up:
                {
                    Text_Height++;
                    Render();
                    break;
                }
            }

            break;
        }
    }

    return GDecorated_Window::Callback_Func(Event);
}
