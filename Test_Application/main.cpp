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

GTexture Load_Texture(const GString& Str) {
    GTexture Texture;
    glGenTextures(1, &Texture.ID);
    glBindTexture(GL_TEXTURE_2D, Texture.ID);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    unsigned char* Data = stbi_load(Str.c_str(), &Texture.Width, &Texture.Height, &Texture.Channels, 0);
    if (Data) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        if (Texture.Channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Texture.Width, Texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Texture.Width, Texture.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        GError() << "Failed to load texture";
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(Data);

    return Texture;
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
Window* Main_Wind;
void App::On_Startup() {
    GLog_Manager::Set_Device(GLog_Device::Std_Console);
    GLog_Manager::Set_Log_Level(GLog_Level::Trace);
    GInfo() << "Helu from Application :*)";

    double Scale = 70;
    Scale /= 100;

    Main_Wind = new Window("Main_Wind", 300 * Scale, 400 * Scale, 1100, 500);
    Register_Window(Main_Wind);
    
    GEvent Render_Event;
    Render_Event.Core_Message = GECore_Message::Render;
    Render_Event.Data_Ptr = Main_Wind;
    
    GApp()->Post_Event(Render_Event);
    GApp()->Post_Event(Render_Event);
}
void App::On_Close() {}


GTexture Textures[32];
GText Text;
int Window::Callback_Func(const GEvent* Event) {
    switch (Event->Type) {
        case GEType::Window:
        {
            if (Event->Wind_Message == GEWind_Message::Run) {
                Set_Default_Font(Load_Font("C:/Windows/Fonts/segoeui.ttf"));

                //Set_Default_Font(Load_Font("C:/Windows/Fonts/consola.ttf"));

                Set_Text_Height(20);
                //Set text height -> store the height in 'default' GFont struct; later for scaling

                Text.push_back({ '!', {1.0, 0.0, 1.0}});
            }

            else if (Event->Wind_Message == GEWind_Message::Render) {
                GRenderer& Renderer = *Get_Renderer();
                Renderer.Clear();
                
                //Default
                GWindow::Callback_Func(Event);
                
                //Render text
                Renderer.Draw_Text(Text, { 0, 0 });
                Renderer.Flush();
                return 0;
            }

            else if (Event->Wind_Message == GEWind_Message::Close) {
                std::cout << "On_Close: Terminate application" << std::endl;
                GEvent Event;
                Event.Core_Message = GECore_Message::Terminate;
                GApp()->Post_Event(Event);

                return 0;
            }

            break;
        }

        case GEType::Keyboard:
        {
            if (Event->Keyboard_Message == GEKeyboard_Message::Key) {
                if (Event->Key_Action == GEKey_Action::Up) {
                    static int Scale = 25;
                    Set_Text_Height(Scale++);

                    Text.clear();
                    std::stringstream SS;
                    SS << Scale;
                    std::string Str = SS.str();

                    for (auto& Ch : Str) {
                        Text.push_back({ Ch, {1.0, 0.0, 1.0} });
                    }
                    GInfo() << "Scale: " << Scale;
                    Text.push_back({ 'b', {0.0, 1.0, 0.7} });

                    GEvent Event;
                    Event.Core_Message = GECore_Message::Render;
                    Event.Data_Ptr = this;
                    GApp()->Post_Event(Event);
                    return 0;
                }
            }
        }
    }

    return GWindow::Callback_Func(Event);
}
