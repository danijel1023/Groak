#include "GWindow.h"
#include "GApplication.h"
#include "GLog_Manager.h"


GWindow::GWindow(const GString& Name, int Window_X, int Window_Y)
    : GWindow(Name, { Window_X, Window_Y }) {}

GWindow::GWindow(const GString& Name, const GSize& Window)
    : GBasic_Window(nullptr, Window, { 0, 0 }), m_Name(Name) {
    m_Main_Window = this;
    m_Dispatcher_Ptr = &GWindow::Dispatcher_Func;
    m_Callback_Ptr = &GWindow::Callback_Func;
}


GWindow::~GWindow() {
    for (auto& Font : m_Font_List) {
        FT_Done_Face(Font->Face);
        delete Font;
    }

    for (auto& Texture : m_Texture_List) {
        glGenTextures(1, &Texture.ID);
    }
}



int GWindow::Send_Event(const GEvent& Event) {
    std::unique_lock<std::recursive_mutex> Lck(m_Dispatcher_Mutex);
    return GCall(this, m_Dispatcher_Ptr, Event);
}

void GWindow::Post_Event(const GEvent& Event) {
    auto Node = m_Queue.Get_Node();
    Node->Data = Event;
    m_Queue.Insert(Node);

    m_DCV.notify_all();
}


void GWindow::Set_Focus(GBasic_Window* Window) {
    if (!Window) { GError() << "Invalid action: Set_Focus cannot be NULL!"; return; }
    if (m_Focus == Window) return;

    GEvent Event;
    Event.Type = GEType::Window;

    Event.Wind_Message = GEWind_Message::Lose_Focus;
    GCall(m_Focus, m_Callback_Ptr, Event);

    Event.Wind_Message = GEWind_Message::Gain_Focus;
    GCall(Window, m_Callback_Ptr, Event);

    m_Focus = Window;
}



int GWindow::Callback_Func(void* _This, const GEvent& Event) {
    auto This = static_cast<GWindow*>(_This);
    return This->Callback_Func(Event);
}

int GWindow::Dispatcher_Func(void* _This, const GEvent& Event) {
    auto This = static_cast<GWindow*>(_This);
    return This->Dispatcher_Func(Event);
}

int GWindow::Dispatcher_Func(const GEvent& Event) {
    switch (Event.Type) {
        case GEType::Custom:
        {
            auto Wind = static_cast<GBasic_Window*>(Event.Data_Ptr);
            return GCall(Wind, m_Callback_Ptr, Event);
        }

        case GEType::Renderer:
        {
            m_Renderer->Post_Event(Event);
            break;
        }

        case GEType::Keyboard:
        {
            return GCall(m_Focus, m_Callback_Ptr, Event);
        }

        case GEType::Mouse:
        {
            if (m_Mouse_Focus) {
                GEvent ncEvent = Event;
                ncEvent.MP -= m_Mouse_Focus->m_Absolute_Screen;

                if (ncEvent.Mouse_Message == GEMouse_Message::Down)
                    m_Main_Window->m_Mouse_Buttons_Pressed++;

                if (ncEvent.Mouse_Message == GEMouse_Message::Up)
                    m_Mouse_Buttons_Pressed--;


                if (m_Mouse_Buttons_Pressed != 0)
                    return GCall(m_Mouse_Focus, m_Callback_Ptr, ncEvent);
                else {
                    GEvent Lose_Focus;
                    Lose_Focus.Type = GEType::Mouse;
                    Lose_Focus.Mouse_Message = GEMouse_Message::Lose_Focus;
                    GCall(m_Mouse_Focus, m_Callback_Ptr, Lose_Focus);
                }
            }

            // Its only possible to recieve here the "Leave" event because
            // that event gets generated in glfw callback.
            // It gets generated only when the cursor is outside of the OS window.
            if (Event.Mouse_Message == GEMouse_Message::Leave) {
                if (m_Wind_Under_Cursor) {
                    GEvent Leave;
                    Leave = Event;
                    Leave.MP -= m_Wind_Under_Cursor->m_Absolute_Screen;
                    GCall(m_Wind_Under_Cursor, m_Callback_Ptr, Leave);
                }

                m_Wind_Under_Cursor = nullptr;
                return 1;
            }

            break;
        }
    }

    return GBasic_Window::Dispatcher_Func(Event);
}

int GWindow::Callback_Func(const GEvent& Event) {
    //GApp()->Resolve_Event(Event, &std::cout);

    switch (Event.Type) {
        case GEType::Window:
        {
            switch (Event.Wind_Message) {
                case GEWind_Message::Gain_Focus:
                {
                    m_Focused = true;
                    break;
                }

                case GEWind_Message::Lose_Focus:
                {
                    m_Focused = false;
                    break;
                }

                case GEWind_Message::Move:
                {
                    m_Screen = Event.WP;
                    return 0;
                }

                case GEWind_Message::Close:
                {
                    GEvent Event;
                    Event.Core_Message = GECore_Message::Close;
                    Event.Data_Ptr = this;
                    GApp()->Post_Event(Event);

                    return 1;
                }

                case GEWind_Message::Should_Iconify:
                {
                    GEvent Event;
                    Event.Core_Message = GECore_Message::Iconify;
                    Event.Data_Ptr = this;
                    GApp()->Post_Event(Event);

                    return 1;
                }

                case GEWind_Message::Should_Maximise:
                {
                    GEvent Event;
                    Event.Core_Message = GECore_Message::Maximise;
                    Event.Data_Ptr = this;
                    GApp()->Post_Event(Event);

                    return 1;
                }

                case GEWind_Message::Should_Restore:
                {
                    GEvent Event;
                    Event.Core_Message = GECore_Message::Restore;
                    Event.Data_Ptr = this;
                    GApp()->Post_Event(Event);
                    return 0;
                }
            }

            break;
        }
    }

    return GBasic_Window::Callback_Func(Event);
}



void GWindow::Run() {
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 32);
    m_Window_Hndl = glfwCreateWindow(m_Window.X, m_Window.Y, m_Name.c_str(), NULL, NULL);
    if (!m_Window_Hndl) {
        const char* Error_Msg;
        glfwGetError(&Error_Msg);
        GError() << "Could not create window: " << m_Name;
        GError() << Error_Msg;
        return;
    }

    glfwMakeContextCurrent(m_Window_Hndl);

    if (!gladLoadGL()) {
        GError() << "GLAD: Failed to initialize OpenGL context";
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    GApp()->Attach_Callbacks(this);

    m_Renderer = new GRenderer(this, m_Window_Hndl);
    m_Renderer->Set_Window_Space({ 0, 0 }, m_Window);

    glClearColor((25.0f / 255.0f), (40.0f / 255.0f), (90.0f / 255.0f), 1.0f);
    glfwMakeContextCurrent(0);


    m_Worker = std::thread(&GWindow::Worker, this);

    GEvent Run_E;
    Run_E.Type = GEType::Window;
    Run_E.Wind_Message = GEWind_Message::Run;
    Post_Event(Run_E);
}

void GWindow::Worker() {
    m_Renderer->Start_Thread();

    while (m_Running) {
        std::unique_lock<std::recursive_mutex> Lck(m_Dispatcher_Mutex);
        m_DCV.wait(Lck, [=] { return !m_Queue.Empty(); });

        while (!m_Queue.Empty()) {
            GEvent Event = m_Queue.Peek_Front();
            GCall(this, m_Dispatcher_Ptr, Event);

            if (Event.Type == GEType::Window && Event.Wind_Message == GEWind_Message::Terminate_Thread)
                m_Running = false;

            m_Queue.Pop();
        }
    }
    
    m_Renderer->Join_Thread();
}


void GWindow::Render() {
    GEvent Event;
    Event.Renderer_Message = GERenderer_Message::Render;
    m_Renderer->Post_Event(Event);
}

void GWindow::Terminate() {
    GEvent Event;
    Event.Type = GEType::Window;
    Event.Wind_Message = GEWind_Message::Terminate_Thread;

    Post_Event(Event);
    m_Worker.join();
}



GTexture GWindow::Load_Texture(const GString& Path, bool Flip) {
    GTexture Texture;
    stbi_set_flip_vertically_on_load(Flip);
    unsigned char* Data = stbi_load(Path.c_str(), &Texture.Width, &Texture.Height, &Texture.Channels, 0);
    if (Data) Texture.ID = m_Renderer->Store_Texture(Data, Texture);
    else      std::cout << "Failed to load texture: " << Path;
    //else      GError() << "Failed to load texture: " << Path;

    //if (Data) {
    //    std::fstream File(Path.cpp_str() + ".dat", std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    //
    //    File.write((char*)Data, (size_t)Texture.Width * (size_t)Texture.Height * (size_t)Texture.Channels);
    //}

    m_Texture_List.push_back(Texture);
    stbi_image_free(Data);
    return Texture;
}

GTexture GWindow::Load_Texture_From_Memory(const unsigned char* Mem_Data, unsigned int Size, bool Flip) {
    GTexture Texture;
    stbi_set_flip_vertically_on_load(Flip);
    unsigned char* Data = stbi_load_from_memory(Mem_Data, Size, &Texture.Width, &Texture.Height, &Texture.Channels, 0);
    if (Data) Texture.ID = m_Renderer->Store_Texture(Data, Texture);
    else      std::cout << "Failed to load texture from memory.";
    //else      GError() << "Failed to load texture from memory.";

    m_Texture_List.push_back(Texture);
    stbi_image_free(Data);
    return Texture;
}



GFont* GWindow::Load_Font(const GString& Font_File) {
    GFont* Font = new GFont;
    Font->File = Font_File;

    auto Error = FT_New_Face(GApp()->Get_FT_Lib(), Font_File.c_str(), 0, &Font->Face);
    if (Error == FT_Err_Unknown_File_Format) {
        GError() << Font_File
            << " - FreeType: The font file could be opened and read, but it appears that its font format is unsupported.";
        delete Font;
        return nullptr;
    }
    else if (Error) {
        GError() << Font_File
            << " - FreeType: Unknown error code means that the font file could not be opened or read, or that it is broken.";
        delete Font;
        return nullptr;
    }

    FT_Set_Pixel_Sizes(Font->Face, 0, 58);

    Font->Height = Font->Face->size->metrics.height >> 6;
    Font->Descender = -(Font->Face->size->metrics.descender >> 6);
    Font->Max_Advance_Width = Font->Face->size->metrics.max_advance >> 6;


    GAtlas* Atlas = nullptr;
    int X_Offset = 0, Y_Offset = 0;
    unsigned int Ch = 0, Prev_Ch = 0;

    unsigned int Index = 0;
    Ch = FT_Get_First_Char(Font->Face, &Index);
    Prev_Ch = Ch;

    Atlas = &Font->Atlas_List.emplace_back();
    Atlas->Min = Ch;

    while (Index != 0) {
        if (FT_Load_Char(Font->Face, Ch, FT_LOAD_RENDER)) {
            GError() << "FreeType: Failed to load Glyph";
        }

        int Advance = Font->Face->glyph->advance.x >> 6;
        if (X_Offset + Advance >= 2048) {
            X_Offset = 0;
            Y_Offset += Font->Height;
        }

        if (Y_Offset + Font->Height >= 2048) {
            Atlas->Max = Prev_Ch;
            Y_Offset = 0;

            Atlas = &Font->Atlas_List.emplace_back();
            Atlas->Min = Ch;
        }

        X_Offset += Advance;
        Prev_Ch = Ch;
        Ch = FT_Get_Next_Char(Font->Face, Ch, &Index);
    }

    Atlas->Max = Prev_Ch;

    m_Font_List.push_back(Font);
    return Font;
}

GFont* GWindow::Set_Default_Font(GFont* Font) {
    auto Last_Default_Font = m_Default_Font;
    m_Default_Font = Font;
    return Last_Default_Font;
}

GFont* GWindow::Get_Default_Font() {
    return m_Default_Font;
}
