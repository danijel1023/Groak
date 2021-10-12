#include "GConsole/GConsole.h"
#include "GApplication.h"
#include "GLog_Manager.h"


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


static GText Text;
static int Text_Height = 20;
static GFont* Consola;

int GConsole::Callback_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Console:
        {
            switch (Event.Console_Message) {
                case GEConsole_Message::Sync:
                {
                    std::unique_lock<std::mutex> Lck(m_Render_Sync_Mutex);

                    m_Render_Text.clear();
                    for (size_t i = 0; i < m_Buffer.size(); i++) {
                        auto& Buffer_String = m_Buffer[i];

                        GColor Color;
                        if      (Buffer_String.substr(0, 7) == "[Trace]")   Color = { 0, 55, 218, 255 };
                        else if (Buffer_String.substr(0, 6) == "[Info]")    Color = { 97, 214, 214, 255 };
                        else if (Buffer_String.substr(0, 9) == "[Warning]") Color = { 193, 156, 0, 255 };
                        else if (Buffer_String.substr(0, 7) == "[Error]")   Color = { 231, 72, 86, 255 };
                        else if (Buffer_String.substr(0, 7) == "[Fatal]")   Color = { 197, 15, 31, 255 };
                        else                                                Color = { 204, 204, 204, 255 };

                        m_Render_Text.emplace_back();
                        auto& Render_String = m_Render_Text.back();
                        for (size_t Ch = 0; Ch < Buffer_String.size(); Ch++) {
                            Render_String.push_back({ (int)Buffer_String[Ch], Color });
                        }
                    }

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
                    GQuad Quad({ 2, 100 }, { 50, 0 });
                    Quad.m_Color = { 255, 0, 0, 255 };
                    Quad.m_Rotation = 31;

                    Add_Quad(Quad);
                    Render();

                    Consola = Load_Font("C:/Windows/Fonts/consola.ttf");

                    Text.push_back({ 'R', {240, 245, 64, 255} });
                    Text.push_back({ 'e', {235, 64, 51, 255} });
                    Text.push_back({ 'c', {255, 0, 255, 255} });
                    Text.push_back({ 'o', {255, 0, 255, 255} });
                    Text.push_back({ 'r', {255, 0, 255, 255} });
                    Text.push_back({ 'd', {255, 0, 255, 255} });
                    Text.push_back({ 'i', {255, 0, 255, 255} });
                    Text.push_back({ 'n', {255, 0, 255, 255} });
                    Text.push_back({ 'g', {255, 0, 255, 255} });
                    Text.push_back({ '.', {255, 0, 255, 255} });
                    Text.push_back({ '.', {255, 0, 255, 255} });
                    Text.push_back({ 272, {255, 0, 255, 255} });
                    
                    GInfo() << "Constructig GConsole...";

                    GTrace() << "Testing Trace mode";
                    GInfo() << "Testing Info mode";
                    GWarning() << "Testing Warning mode";
                    GError() << "Testing Error mode";
                    GFatal() << "Testing Fatal mode";

                    GLog_Manager() << "Testing \"arbitrary\" mode";

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

                case GEWind_Message::Render:
                {
                    GRenderer& Renderer = *Get_Renderer();
                    Renderer.Clear();

                    //Default
                    GWindow::Callback_Func(Event);

                    //Render text
                    std::unique_lock<std::mutex> Lck(m_Render_Sync_Mutex);
                    for (int i = 0; i < m_Render_Text.size(); i++) {
                        Renderer.Draw_Text(m_Render_Text.at(i), { 50, m_Window.Y - Get_Title_Bar_Window().Y - (Text_Height * (i+1)) }, Text_Height);
                        //Renderer.Draw_Text(m_Render_Text.at(i), { 50, m_Window.Y - Get_Title_Bar_Window().Y - (Text_Height * (i+1)) }, Text_Height, Consola);
                    }

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
                    Event.Type = GEType::Core;
                    Event.Core_Message = GECore_Message::Terminate;
                    GApp()->Post_Event(Event);
                }

                if (Event.Key_Action == GEKey_Action::Down && Event.Key == 82) {
                    Event.Resolve_Event(GInfo().NLE().Get_Stream());
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
                    Event.Cursor_Type = GCursor_Type::Not_Allowed;
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
