#include <iostream>
#include <sstream>
#include "Groak.h"
#include "GCharacter.h"


class App : public GApplication {
public:
    App() { On_Startup(); }
    ~App() { On_Close(); }

    void On_Startup();
    void On_Close();
};
int main() {
    App Application;
    return Application.Run();
}


class Window : public GWindow {
public:
    Window(const std::string& Name, int Window_X, int Window_Y)
        : GWindow(Name, Window_X, Window_Y) {
        m_Callback_Ptr = &Window::Callback_Func;
    }


protected:
    int Callback_Func(const GEvent& Event);

private:
    static int Callback_Func(void* _This, const GEvent& Event) {
        auto This = static_cast<Window*>(_This);
        return This->Callback_Func(Event);
    }
};

Window* Main_Wind;
void App::On_Startup() {
    GLog_Manager::Set_Device(GLog_Device::GConsole, "log.txt");
    GLog_Manager::Set_Log_Level(GLog_Level::Trace);
    GInfo() << "Helu from Application :*)";

    //double Scale = 70;
    //Scale /= 100;
    //
    //Main_Wind = new Window("Main_Wind", 300 * Scale, 400 * Scale);
    //Register_Window(Main_Wind);
    //
    //GEvent Render_Event;
    //Render_Event.Core_Message = GECore_Message::Render;
    //Render_Event.Data_Ptr = Main_Wind;
    //GApp()->Post_Event(Render_Event);
}
void App::On_Close() {}


GPos MP;
bool Track = false;
GText Text;
size_t Quad_i = 0;
int Window::Callback_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Window:
        {
            if (Event.Wind_Message == GEWind_Message::Run) {
                //Set_Default_Font(Load_Font("C:/Windows/Fonts/segoeui.ttf"));
                Set_Default_Font(Load_Font("C:/Windows/Fonts/consola.ttf"));

                Set_Text_Height(20);
                Text.push_back({ '!', {1.0, 0.0, 1.0}});

                GTexture Tex = Load_Texture("C:/test.bmp");
                GQuad Quad(Tex.Width * (5.2 / 100.0), Tex.Height * (5.2 / 100.0), 0, 75);
                Quad.Repeat_Texture(Tex, 1, 1);
                Quad_i = Add_Quad(Quad);

                //Bkg color: 30, 30, 30, 255
            }

            else if (Event.Wind_Message == GEWind_Message::Render) {
                GRenderer& Renderer = *Get_Renderer();
                Renderer.Clear();
                
                //Default
                GWindow::Callback_Func(Event);
                
                //Render text
                Renderer.Draw_Text(Text, { 0, 0 });
                Renderer.Flush();
                return 0;
            }

            else if (Event.Wind_Message == GEWind_Message::Close) {
                GInfo() << "Close event: Application termination";
                GEvent Event;
                Event.Core_Message = GECore_Message::Terminate;
                GApp()->Post_Event(Event);

                return 0;
            }

            break;
        }

        case GEType::Keyboard:
        {
            if (Event.Keyboard_Message == GEKeyboard_Message::Key) {
                if (Event.Key_Action == GEKey_Action::Down || Event.Key_Action == GEKey_Action::Repeat) {
                    static int Scale = 25;

                    if (265 >= Event.Key && Event.Key >= 262) {
                        auto& Quad = Get_Quad(Quad_i);

                        if (Event.Key == 265) {
                            Quad.m_Window.X += int((double)Quad.m_Window.X * 0.02);
                            Quad.m_Window.Y += int((double)Quad.m_Window.Y * 0.02);
                            Set_Text_Height(++Scale);
                        } else if (Event.Key == 264) {
                            Quad.m_Window.X -= int((double)Quad.m_Window.X * 0.02);
                            Quad.m_Window.Y -= int((double)Quad.m_Window.Y * 0.02);
                            Set_Text_Height(--Scale);
                        }
                        
                        else if (Event.Key == 263) {
                            Quad.m_Rotation -= 0.5;
                        } else if (Event.Key == 262) {
                            Quad.m_Rotation += 0.5;
                        }


                        GEvent Render;
                        Render.Core_Message = GECore_Message::Render;
                        Render.Data_Ptr = this;
                        GApp()->Post_Event(Render);

                        Text.clear();
                        std::stringstream SS;
                        SS << Scale;
                        std::string Str = SS.str();

                        for (auto& Ch : Str) {
                            Text.push_back({ Ch, {1.0, 0.0, 1.0} });
                        }
                        GInfo() << "Scale: " << Scale;

                        return 0;
                    }

                    else if (Event.Key == 71) {
                        GLog_Manager::Set_Device(GLog_Device::GConsole);
                    }

                    else if (Event.Key == 83) {
                        GLog_Manager::Set_Device(GLog_Device::Std_Console);
                    }
                }
            }

            break;
        }

        case GEType::Mouse:
        {
            if (Event.Mouse_Message == GEMouse_Message::Move) {
                //GInfo() << "Mouse move (" << Get_Name() << ") (x, y): [" << Event.MP.X << ", " << Event.MP.Y << "]";

                if (Track) {
                    GEvent Move_E;
                    Move_E.Data_Ptr = m_Main_Window;
                    Move_E.Core_Message = GECore_Message::Move;

                    Move_E.WP = m_Screen;
                    Move_E.WP.X += Event.MP.X - MP.X;
                    Move_E.WP.Y -= Event.MP.Y - MP.Y;
                    GApp()->Send_Event(Move_E);
                }
                else MP = Event.MP;
            }

            else if (Event.Mouse_Button == GEMouse_Button::LMB) {
                if (Event.Mouse_Message == GEMouse_Message::Down) {
                    Track = true;
                    Set_Focus(this);
                }

                else if (Event.Mouse_Message == GEMouse_Message::Up) {
                    Track = false;
                }
            }

            return 1;
        }
    }

    return GWindow::Callback_Func(Event);
}
