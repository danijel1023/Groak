#include "GConsole/GConsole.h"
#include "GApplication.h"
#include "GLog_Manager.h"

#define Window_Icon_Event(Event, Array_Ptr, Count)\
(Event).Type = GEType::Core;\
(Event).Core_Message = GECore_Message::Set_Window_Icon;\
(Event).Data_Ptr = this;\
(Event).Texture_Count = Count;\
(Event).Texture_Arrays = Array_Ptr;

#include <chrono>
#include <thread>

GConsole::GConsole()
    : GDecorated_Window("Groak Console", 500, 500), m_Stream_Buff(this), m_Stream(&m_Stream_Buff) {
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
        case GEType::Console:
        {
            switch (Event.Console_Message) {
                case GEConsole_Message::Sync:
                {
                    GEvent Event;
                    Event.Type = GEType::Console;
                    Event.Console_Message = GEConsole_Message::Update_Rendered_Text;
                    Post_Event(Event);

                    std::unique_lock<std::mutex> Lck(m_Buffer_Mutex);
                    for (const auto& Line : m_Buffer)
                        std::cout << Line;

                    m_Buffer.clear();
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

                    Render();
                    break;
                }
            }

            break;
        }

        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Run:
                {
                    GDecorated_Window::Callback_Func(Event);
                    m_Main_Window->Set_Default_Font(m_Main_Window->Load_Font("C:/Windows/Fonts/Consola.ttf"));
                    //m_Main_Window->Set_Default_Font(m_Main_Window->Load_Font("C:/Users/RI_Elev/Desktop/Cousine/Cousine-Regular.ttf"));

                    m_Text_Box = new GText_Box(this, { m_Window.X, m_Window.Y - Get_Title_Bar_Window().Y }, {0, 0});
                    m_Text_Box->m_Text_Height = 40;


                    m_Text_Box->m_Buffer_Mutex.lock();
                    
                    m_Text_Box->m_Text.push_back("0: iiiii This is some random line");
                    m_Text_Box->m_Text.push_back("1: Today was such a nice sunny day");
                    m_Text_Box->m_Text.push_back("2: I hate sun.. it's always so hot");
                    m_Text_Box->m_Text.push_back("3: This morning was nice and cold");
                    m_Text_Box->m_Text.push_back("4: By drinking water, you wont die");
                    m_Text_Box->m_Text.push_back("5: Breathing air is important");
                    m_Text_Box->m_Text.push_back("6: This is some random line");
                    m_Text_Box->m_Text.push_back("7: Today was such a nice sunny day");
                    m_Text_Box->m_Text.push_back("8: I hate sun.. it's always so hot");
                    m_Text_Box->m_Text.push_back("9: This morning was nice and cold");
                    m_Text_Box->m_Text.push_back("A: By drinking water, you wont die");
                    m_Text_Box->m_Text.push_back("B: Breathing air is important");

                    for (size_t Line = 0; Line < m_Text_Box->m_Text.size(); Line++) {
                        m_Text_Box->m_Text_Color.emplace_back();

                        for (size_t Ch = 0; Ch < m_Text_Box->m_Text[Line].length(); Ch++)
                            m_Text_Box->m_Text_Color.back().push_back({ 97, 214, 214, 255 });
                    }

                    m_Text_Box->m_Buffer_Mutex.unlock();


                    //Load_Font("C:/Windows/Fonts/consola.ttf");
                    //m_Texture = Groak::Load_Texture("C:/Users/comp/Desktop/asd.png", true);
                    //Store_Texture(&m_Texture);
                    //
                    //GQuad Quad(m_Window.Cast<GVec2>(), m_Screen.Cast<GVec2>());
                    //Quad.m_Color = { 193, 156, 0, 255 };
                    //Quad.Repeat_Texture(m_Texture, (m_Window.X / m_Texture.Size.X) + 1, (m_Window.Y / m_Texture.Size.Y) + 1);
                    //Add_Quad(Quad);


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

                    return 1;
                }

                case GEWind_Message::Render:
                {
                    GRenderer* Renderer = m_Main_Window->Get_Renderer();
                    Renderer->Clear();

                    break;
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

        case GEType::Keyboard:
        {
            //Key: Up(265), Down(264), Left(263), Right(262)
            switch (Event.Keyboard_Message) {
                case GEKeyboard_Message::Key:
                {
                    if (Event.Key_Action == GEKey_Action::Down && Event.Key == 265) {
                        GEvent Event;
                        Event.Type = GEType::Core;
                        Event.Core_Message = GECore_Message::Terminate;
                        //GApp()->Post_Event(Event);
                    }

                    if (Event.Key_Action == GEKey_Action::Down || Event.Key_Action == GEKey_Action::Repeat) {
                        switch (Event.Key) {
                            case 265:
                            {
                                auto& Y = m_Text_Box->m_Text_Offset.Y;
                                Y -= 0.5f;

                                Render();
                                break;
                            }
                            case 264:
                            {
                                auto& Y = m_Text_Box->m_Text_Offset.Y;
                                Y += 0.5f;

                                Render();
                                break;
                            }
                            case 263:
                            {
                                auto& X = m_Text_Box->m_Text_Offset.X;
                                X++;

                                Render();
                                break;
                            }
                            case 262:
                            {
                                auto& X = m_Text_Box->m_Text_Offset.X;
                                X--;

                                Render();
                                break;
                            }
                        }
                    }

                    break;
                }

                case GEKeyboard_Message::Text:
                {
                    break;
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
                    auto& TH = m_Text_Box->m_Text_Height;
                    TH = TH - (TH * (1.0f / 100.0f));

                    Render();


                    //if (m_Modifier_Ctrl) {
                    //    if (m_Text_Height)
                    //        m_Text_Height--;
                    //}
                    //
                    //else {
                    //    if (m_Modifier_Shift)
                    //        m_Text_Offset.X--;
                    //    else
                    //        m_Text_Offset.Y -= m_Text_Height;
                    //}
                    //
                    //Render();
                    break;
                }

                case GEMouse_Message::Scroll_Up:
                {
                    auto& TH = m_Text_Box->m_Text_Height;
                    TH = TH + (TH * (1.0f / 100.0f));

                    Render();

                    //if (m_Modifier_Ctrl) {
                    //    if (m_Text_Height)
                    //        m_Text_Height++;
                    //}
                    //
                    //else {
                    //    if (m_Modifier_Shift)
                    //        m_Text_Offset.X++;
                    //    else
                    //        m_Text_Offset.Y += m_Text_Height;
                    //}
                    //
                    //Render();
                    break;
                }
            }

            break;
        }
    }

    return GDecorated_Window::Callback_Func(Event);
}
