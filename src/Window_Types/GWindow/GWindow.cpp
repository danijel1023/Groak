#include "GWindow.h"
#include "GApplication.h"
#include "GLog_Manager.h"

GWindow::GWindow(const GString& Name, int Window_X, int Window_Y, int Screen_X, int Screen_Y)
    : GBasic_Window(nullptr, Window_X, Window_Y, Screen_X, Screen_Y), m_Name(Name) {
    m_Main_Window = this;
    m_Dispatcher_Ptr = &GWindow::Dispatcher_Func;
    m_Callback_Ptr = &GWindow::Callback_Func;
}

GWindow::~GWindow() {
    for (auto Font : m_Font_List) {
        FT_Done_Face(Font->Face);
        delete Font;
    }
}



int GWindow::Send_Event(const GEvent& Event) {
    std::unique_lock<std::recursive_mutex> Lck(m_Dispatcher_Mutex);
    return GCall(this, m_Dispatcher_Ptr, &Event);
}

void GWindow::Post_Event(const GEvent& Event) {
    auto Node = m_Queue.Get_Node();
    Node->Data = Event;
    m_Queue.Insert(Node);

    m_DCV.notify_all();
}


void GWindow::Set_Focus(GBasic_Window* Window) {
    m_Focus = Window;
}



int GWindow::Callback_Func(void* _This, const GEvent* Event) {
    auto This = static_cast<GWindow*>(_This);
    return This->Callback_Func(Event);
}

int GWindow::Dispatcher_Func(void* _This, const GEvent* Event) {
    auto This = static_cast<GWindow*>(_This);
    return This->Dispatcher_Func(Event);
}

int GWindow::Dispatcher_Func(const GEvent* Event) {
    switch (Event->Type) {
        case GEType::Custom:
        {
            auto Direct_This = static_cast<GBasic_Window*>(Event->Data_Ptr);
            return GCall(Direct_This, m_Callback_Ptr, Event);
        }

        case GEType::Keyboard:
        {
            return GCall(m_Focus, m_Callback_Ptr, Event);
        }

        case GEType::Mouse:
        {
            if (Event->Mouse_Message == GEMouse_Message::Move) {
                if (m_Mouse_Focus) {
                    return GCall(m_Mouse_Focus, m_Callback_Ptr, Event);
                }
            }

            else if (Event->Mouse_Message == GEMouse_Message::Leave) {
                if (m_Wind_Under_Cursor) GCall(m_Wind_Under_Cursor, m_Callback_Ptr, Event);
                m_Wind_Under_Cursor = nullptr;
                return 1;
            }

            break;
        }

        case GEType::Window:
        {
            if (Event->Wind_Message == GEWind_Message::Move) {
                return GCall(this, m_Callback_Ptr, Event);
            }

            break;
        }
    }

    return GBasic_Window::Dispatcher_Func(Event);
}

int GWindow::Callback_Func(const GEvent* Event) {
    if (Event->Type == GEType::Window) {
        switch (Event->Wind_Message) {
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
                m_Screen_X = Event->WP.X;
                m_Screen_Y = Event->WP.Y;
                return 0;
            }

            case GEWind_Message::Close:
            {
                GEvent Event;
                Event.Core_Message = GECore_Message::Close;
                Event.Data_Ptr = this;
                GApp()->Post_Event(Event);

                return 0;
            }
        }
    }

    return GBasic_Window::Callback_Func(Event);
}



void GWindow::Run() {
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 32);
    m_Window_Hndl = glfwCreateWindow(m_Window_X, m_Window_Y, m_Name.c_str(), NULL, NULL);
    if (!m_Window_Hndl) {
        const char* Error_Msg;
        glfwGetError(&Error_Msg);
        GError() << "Could not create window: " << m_Name;
        GError() << Error_Msg;
        return;
    }

    GApp()->Set_Context(this);

    if (!gladLoadGL()) {
        GError() << "GLAD: Failed to initialize OpenGL context";
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    glfwSetWindowPos(m_Window_Hndl, m_Screen_X, m_Screen_Y);

    GApp()->Attach_Callbacks(this);

    m_Renderer = new GRenderer(this);
    m_Renderer->Set_Window_Space(0, 0, m_Window_X, m_Window_Y);

    glClearColor((0.0f / 255.0f), (0.0f / 255.0f), (255.0f / 255.0f), 1.0f);
    m_Renderer->Clear();
    glfwSwapBuffers(m_Window_Hndl);
    m_Renderer->Clear();
    glfwSwapBuffers(m_Window_Hndl);

    m_Worker = std::thread(&GWindow::Worker, this);

    GEvent Run_E;
    Run_E.Type = GEType::Window;
    Run_E.Wind_Message = GEWind_Message::Run;
    Send_Event(Run_E);
}

void GWindow::Worker() {
    while (m_Running) {
        std::unique_lock<std::recursive_mutex> Lck(m_Dispatcher_Mutex);
        m_DCV.wait(Lck, [=] { return !m_Queue.Empty(); });

        while (!m_Queue.Empty()) {
            GEvent Event = m_Queue.Peek_Front();
            GCall(this, m_Dispatcher_Ptr, &Event);

            if (Event.Type == GEType::Window && Event.Wind_Message == GEWind_Message::Terminate_Thread)
                m_Running = false;

            m_Queue.Pop();
        }
    }
}


void GWindow::Terminate() {
    GEvent Event;
    Event.Type = GEType::Window;
    Event.Wind_Message = GEWind_Message::Terminate_Thread;

    Post_Event(Event);
    m_Worker.join();
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
            << " - FreeType: another error code means that the font file could not be opened or read, or that it is broken.";
        delete Font;
        return nullptr;
    }

    FT_Set_Pixel_Sizes(Font->Face, 0, 58);

    Font->Height = Font->Face->size->metrics.height >> 6;
    Font->Descender = -(Font->Face->size->metrics.descender >> 6);
    Font->Max_Advance_Width = Font->Face->size->metrics.max_advance >> 6;
    Font->Scale_Height = Font->Height;


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


void GWindow::Set_Text_Height(int Height) {
    if (!m_Default_Font) {
        GError() << "Set_Text_Height(d:" << Height << ") called on no default font";
        return;
    }

    Set_Text_Height(Height, m_Default_Font);
}

void GWindow::Set_Text_Height(int Height, GFont* Font) {
    Font->Scale_Height = Height;
}
