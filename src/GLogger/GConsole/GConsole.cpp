#include "GConsole/GConsole.h"
#include "GApplication.h"
#include "GLog_Manager.h"

#define Window_Icon_Event(Event, Array_Ptr, Count)\
(Event).Type = GEType::Core;\
(Event).Core_Message = GECore_Message::Set_Window_Icon;\
(Event).Data_Ptr = this;\
(Event).Texture_Count = Count;\
(Event).Texture_Arrays = Array_Ptr;



GConsole::GConsole()
    : GDecorated_Window("Groak Console", 500, 250), m_Stream_Buff(this), m_Stream(&m_Stream_Buff) {
    m_Callback_Ptr = &GConsole::Callback_Func;

    //GApp()->Attach_Simulator(this, false);
}

GConsole::~GConsole() {
    GLog_Manager::Close_GConsole();
    GLog_Manager::Set_Device(GLog_Device::Std_Console);
}



void GConsole::Show() {
    GEvent Event;
    Event.Type = GEType::Core;
    Event.Core_Message = GECore_Message::Show;
    Event.Data_Ptr = this;
    GApp()->Post_Event(Event);
}

void GConsole::Hide() {
    GEvent Event;
    Event.Type = GEType::Core;
    Event.Core_Message = GECore_Message::Hide;
    Event.Data_Ptr = this;
    GApp()->Post_Event(Event);
}

std::ostream* GConsole::Get_Stream() { return &m_Stream; }

GPos MP_C;
bool Track_C = false;

GTexture Icon;

float Text_Height = 40;
int GConsole::Callback_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Run:
                {
                    //m_Text_Box = new GText_Box();

                    GQuad Quad({ 2, 100 }, { 50, 0 });
                    Quad.m_Color = { 255, 0, 0, 255 };
                    Quad.m_Rotation = 31;

                    Add_Quad(Quad);
                    Render();

                    //Load_Font("C:/Windows/Fonts/consola.ttf");

                    Icon = Groak::Load_Texture("../../../../Test_Application/res/Image1.png");
                    Store_Texture(&Icon);
                    GEvent Event;
                    Window_Icon_Event(Event, &Icon, 1);
                    GApp()->Post_Event(Event);

                    GTrace() << "Testing Trace mode";
                    GInfo() << "Testing Info mode";
                    GWarning() << "Testing Warning mode";
                    GError() << "Testing Error mode";
                    GFatal() << "Testing Fatal mode";

                    GLog_Manager() << "Testing \"arbitrary\" mode";

                    break;
                }

                case GEWind_Message::Render:
                {
                    GDecorated_Window::Callback_Func(Event);
                    GRenderer* Renderer = m_Main_Window->Get_Renderer();
                    
                    GColor Color = { 242, 245, 66, 255 };
                    std::vector<GColor> Color_Str(20, Color);
                    Renderer->Draw_Str("E32.0p4-5a", Color_Str, { 0, 0 }, Text_Height);
                    Renderer->Flush();

                    return 1;
                }

                case GEWind_Message::Close:
                {
                    GEvent Event;
                    Event.Type = GEType::Core;
                    Event.Core_Message = GECore_Message::Terminate;
                    GApp()->Post_Event(Event);
                    break;
                }
            }

            break;
        }


        case GEType::Console:
        {
            switch (Event.Console_Message) {
                case GEConsole_Message::Sync:
                {
                    GEvent Event;
                    Event.Type = GEType::Console;
                    Event.Console_Message = GEConsole_Message::Update_Rendered_Text;
                    Post_Event(Event);
                    break;
                }

                case GEConsole_Message::Update_Rendered_Text:
                {
                    //std::unique_lock<std::mutex> Text_Box_Lck(m_Text_Box->Get_Mutex());
                    //std::unique_lock<std::mutex> Buffer_Lck(m_Buffer_Mutex);
                    //
                    //m_Render_Text.clear();
                    //for (size_t i = 0; i < m_Buffer.size(); i++) {
                    //    auto& Buffer_String = m_Buffer[i];
                    //
                    //    GColor Color;
                    //    if (Buffer_String.substr(0, 7) == "[Trace]")        Color = { 0, 55, 218, 255 };
                    //    else if (Buffer_String.substr(0, 6) == "[Info]")    Color = { 97, 214, 214, 255 };
                    //    else if (Buffer_String.substr(0, 9) == "[Warning]") Color = { 193, 156, 0, 255 };
                    //    else if (Buffer_String.substr(0, 7) == "[Error]")   Color = { 231, 72, 86, 255 };
                    //    else if (Buffer_String.substr(0, 7) == "[Fatal]")   Color = { 197, 15, 31, 255 };
                    //    else                                                Color = { 204, 204, 204, 255 };
                    //
                    //    m_Render_Text.push_back(Buffer_String);
                    //
                    //    //m_Text_Box->Get_Buffer().;
                    //}

                    //Render();
                    break;
                }
            }

            break;
        }


        case GEType::Keyboard:
        {
            if (Event.Keyboard_Message == GEKeyboard_Message::Key) {
                if (Event.Key_Action == GEKey_Action::Down) {
                    if (Event.Key == 256) {
                        GEvent Event;
                        Event.Type = GEType::Core;
                        Event.Core_Message = GECore_Message::Terminate;
                        GApp()->Post_Event(Event);
                    }
                }


                if ((Event.Key_Action == GEKey_Action::Down || Event.Key_Action == GEKey_Action::Repeat)) {
                    if (Event.Key == 86 && Event.Modifier_Ctrl) {
                        GEvent Clipboard;
                        Clipboard.Type = GEType::Core;
                        Clipboard.Core_Message = GECore_Message::Get_Clipboard;
                        GApp()->Send_Event(Clipboard);

                        GInfo() << Clipboard.String;
                    }
                }

                Event.Resolve_Event(&std::cout);
            }

            break;
        }

        case GEType::Mouse:
        {
            switch (Event.Mouse_Message) {
                case GEMouse_Message::Enter:
                {
                    GEvent Event;
                    Event.Type = GEType::Core;
                    Event.Core_Message = GECore_Message::Set_Cursor;
                    Event.Data_Ptr = m_Main_Window;
                    Event.Cursor_Type = GCursor_Type::Default;
                    GApp()->Post_Event(Event);

                    break;
                }

                case GEMouse_Message::Leave:
                {
                    GEvent Event;
                    Event.Type = GEType::Core;
                    Event.Core_Message = GECore_Message::Set_Cursor;
                    Event.Data_Ptr = m_Main_Window;
                    Event.Cursor_Type = GCursor_Type::Default;
                    GApp()->Post_Event(Event);
                    break;
                }

                case GEMouse_Message::Scroll_Down:
                {
                    if (m_Modifier_Ctrl) {
                        if (Text_Height)
                            Text_Height--;
                    }
                    //
                    //else {
                    //    if (m_Modifier_Shift)
                    //        m_Text_Offset.X--;
                    //    else
                    //        m_Text_Offset.Y -= m_Text_Height;
                    //}
                    //
                    Render();
                    break;
                }

                case GEMouse_Message::Scroll_Up:
                {
                    if (m_Modifier_Ctrl) {
                        if (Text_Height)
                            Text_Height++;
                    }
                    //
                    //else {
                    //    if (m_Modifier_Shift)
                    //        m_Text_Offset.X++;
                    //    else
                    //        m_Text_Offset.Y += m_Text_Height;
                    //}
                    //
                    Render();
                    break;
                }
            }

            break;
        }
    }

    return GDecorated_Window::Callback_Func(Event);
}
