#include <iostream>
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
    Window(const std::string& Name, int Window_X, int Window_Y, int Screen_X = 0, int Screen_Y = 0)
        : GWindow(Name, Window_X, Window_Y, Screen_X, Screen_Y) {
        m_Callback_Ptr = &Window::Callback_Func;
    }


protected:
    int Callback_Func(const GEvent* Event);

private:
    static int Callback_Func(void* _This, const GEvent* Event) {
        auto This = static_cast<Window*>(_This);
        return This->Callback_Func(Event);
    }
};



class Child_Window : public GBasic_Window {
public:
    Child_Window(GBasic_Window* Parent, int Window_X, int Window_Y, int Screen_X, int Screen_Y)
        : GBasic_Window(Parent, Window_X, Window_Y, Screen_X, Screen_Y) {
        m_Callback_Ptr = &Child_Window::Callback_Func;
    }


protected:
    int Callback_Func(const GEvent* Event);

private:
    static int Callback_Func(void* _This, const GEvent* Event) {
        auto This = static_cast<Child_Window*>(_This);
        return This->Callback_Func(Event);
    }
};


Child_Window* Title_Bar;
Window* Main_Wind;
void App::On_Startup() {
    GLog_Manager::Set_Device(GLog_Device::Std_Console);
    GLog_Manager::Set_Log_Level(GLog_Level::Trace);
    GInfo() << "Helu from Application :*)";

    double Scale = 70;
    Scale /= 100;

    Main_Wind = new Window("Main_Wind", 300 * Scale, 400 * Scale, 1100, 500);
    Register_Window(Main_Wind);
    //Attach_Simulator(Main_Wind, true);

    //Register_Window(new Window("Console", 400, 300, 600, 200));
    
    GEvent Event;
    Event.Type = GEType::Custom;
    Event.Data_Ptr = Main_Wind;
    Event.Data = 4025;
    Main_Wind->Post_Event(Event);
    
    GEvent Render_Event;
    Render_Event.Core_Message = GECore_Message::Render;
    Render_Event.Data_Ptr = Main_Wind;
    
    GApp()->Post_Event(Render_Event);
}


void App::On_Close() {

}


GPos MP;
bool Track = false;
int Window::Callback_Func(const GEvent* Event) {
    switch (Event->Type) {
        case GEType::Window:
        {
            if (Event->Wind_Message == GEWind_Message::Run) {
                Title_Bar = new Child_Window(this, m_Window_X, 50, 0, 50);
            }

            else if (Event->Wind_Message == GEWind_Message::Move) {
                //GInfo() << "Window Move (" << This->Get_Name() << "): [x, y] [" << Event->WP.X << ", " << Event->WP.Y << "]";
            }

            else if (Event->Wind_Message == GEWind_Message::Close) {
                std::cout << "On_Close: Terminate application" << std::endl;
                GEvent Event;
                Event.Core_Message = GECore_Message::Terminate;
                GApp()->Post_Event(Event);

                return 1;
            }

            else if (Event->Wind_Message == GEWind_Message::Render) {
                glClear(GL_COLOR_BUFFER_BIT);
            }

            break;
        }

        case GEType::Custom:
        {
            if (Event->Data == 4025) {
                std::cout << "Custom: 4025" << std::endl;
                return 1;
            }
            else {
                std::cout << "Custom: Unknown" << std::endl;
            }

            break;
        }

        case GEType::Mouse:
        {
            if (Event->Mouse_Message == GEMouse_Message::Move) {
                GInfo() << "Mouse move (" << Get_Name() << ") (x, y): [" << Event->MP.X << ", " << Event->MP.Y << "]";

                if (Track) {
                    GEvent Move_E;
                    Move_E.Data_Ptr = this;
                    Move_E.Core_Message = GECore_Message::Move;

                    Move_E.WP = { m_Screen_X, m_Screen_Y };
                    Move_E.WP.X += Event->MP.X - MP.X;
                    Move_E.WP.Y -= Event->MP.Y - MP.Y;
                    GApp()->Send_Event(Move_E);
                }
                else MP = Event->MP;

                return 1;
            }

            else if (Event->Mouse_Message == GEMouse_Message::Enter) {
                std::cout << "Mouse enter (" << Get_Name() << ") (x, y): [" << Event->MP.X << ", " << Event->MP.Y << "]" << std::endl;
                MP = Event->MP;
            }

            else if (Event->Mouse_Message == GEMouse_Message::Leave) {
                std::cout << "Mouse leave (" << Get_Name() << ") (x, y): [" << Event->MP.X << ", " << Event->MP.Y << "]" << std::endl;
            }

            else if (Event->Mouse_Message == GEMouse_Message::Down && Event->Mouse_Button == GEMouse_Button::LMB) {
                Track = true;
                Set_Focus(this);
                return 1;
            }

            else if (Event->Mouse_Message == GEMouse_Message::Up && Event->Mouse_Button == GEMouse_Button::LMB) {
                Track = false;
                return 1;
            }

            else if (Event->Mouse_Message == GEMouse_Message::Up && Event->Mouse_Button == GEMouse_Button::RMB) {
                GEvent Event;
                Event.Core_Message = GECore_Message::Close;
                Event.Data_Ptr = this;
                GApp()->Post_Event(Event);
                return 1;
            }

            break;
        }
    }

    return GWindow::Callback_Func(Event);
}


std::vector<GCharacter> Text;
GQuad* Close_TTB;
GFramebuffer* Titlebar_Fb;
int Child_Window::Callback_Func(const GEvent* Event) {
    switch (Event->Type) {
        case GEType::Window:
        {
            if (Event->Wind_Message == GEWind_Message::Run) {
                Titlebar_Fb = Create_Framebuffer();

                Close_TTB = new GQuad(m_Window_X, m_Window_Y, 0, 0);
                Close_TTB->m_Color = { 1.0, 0.0, 1.0, 1.0 };

                Add_Quad(Close_TTB);
            }


            else if (Event->Wind_Message == GEWind_Message::Render) {
                auto& Renderer = *(m_Main_Window->Get_Renderer());
                Close_TTB->m_Rotation++;

                Titlebar_Fb->Use();
                glClear(GL_COLOR_BUFFER_BIT);
                Set_Viewport();

                for (auto& Quad : m_Quad_List) if (Quad->m_Active) Renderer.Add_Quad(*Quad);
                Renderer.Flush();

                for (auto& Char : Text) {
                    Renderer.Add_Char(Char);
                    // - Generate sprite sheet (if needed)
                    // - Create quad for char
                    //     +> Do a texture offset calculation
                    // - Add quad to buffer
                }
                //Renderer.Flush();

                Titlebar_Fb->Render();
                return 0;
            }

            break;
        }

        case GEType::Mouse:
        {
            if (Event->Mouse_Message == GEMouse_Message::Down) {
                m_Main_Window->Set_Focus(this);


                if (Event->Mouse_Button == GEMouse_Button::MMB) {
                    Text.push_back({ 'A', { 0.0f, 0.0f, 1.0f, 1.0f }, { 0, 0 } });
                }
            }
            

            GEvent Render_Event;
            Render_Event.Core_Message = GECore_Message::Render;
            Render_Event.Data_Ptr = Main_Wind;

            GApp()->Post_Event(Render_Event);

            return 1;
        }

        case GEType::Keyboard:
        {
            if (Event->Keyboard_Message == GEKeyboard_Message::Text) {
                std::cout << GString().push_back(Event->Code_Point) << std::endl;
            }

            else {
                if (Event->Key_Action == GEKey_Action::Down) {
                    GInfo() << (char)Event->Key;
                }
            }

            break;
        }
    }

    return GBasic_Window::Callback_Func(Event);
}
