#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include "Shaders/Shaders.inl"

#include "GCore.h"
#include "GRenderer.h"
#include "GWindow.h"
#include "GLog_Manager.h"

#include "GBasic_Framebuffer.h"
#include "GFramebuffer.h"
#include "GTexture.h"


#define __Quad_Count 256
static unsigned int Create_Shader(const std::string& Vertex, const std::string& Fragment);


GRenderer::GRenderer(GWindow* Main_Wind, GLFWwindow* Window_Hndl)
    : m_Main_Wind(Main_Wind), m_Window_Hndl(Window_Hndl) {
    m_Buffer = new GVertex[__Quad_Count * 4];

    m_Shader = Create_Shader(Vertex_Shader, Fragment_Shader);
    int Samplers[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    glUniform1iv(glGetUniformLocation(m_Shader, "u_Textures"), 16, Samplers);
    m_Scale = glGetUniformLocation(m_Shader, "u_Scale");

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Vertex buffer and stride
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, __Quad_Count * 4 * sizeof(GVertex), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glVertexAttribIPointer(0, 1, GL_INT,             sizeof(GVertex), (void*)offsetof(GVertex, Type));
    glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, sizeof(GVertex), (void*)offsetof(GVertex, Pos));
    glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, sizeof(GVertex), (void*)offsetof(GVertex, Centre));
    glVertexAttribPointer (3, 4, GL_FLOAT, GL_FALSE, sizeof(GVertex), (void*)offsetof(GVertex, Color));
    glVertexAttribPointer (4, 2, GL_FLOAT, GL_FALSE, sizeof(GVertex), (void*)offsetof(GVertex, Tex_Coords));
    glVertexAttribIPointer(5, 1, GL_INT,             sizeof(GVertex), (void*)offsetof(GVertex, Tex_id));
    glVertexAttribPointer (6, 1, GL_FLOAT, GL_FALSE, sizeof(GVertex), (void*)offsetof(GVertex, Rotation));

    // Index (element) buffer
    unsigned int Indices[__Quad_Count * 6];
    for (unsigned int i = 0; i < __Quad_Count; i++) {
        Indices[(i * 6) + 0] = (i * 4) + 0;
        Indices[(i * 6) + 1] = (i * 4) + 1;
        Indices[(i * 6) + 2] = (i * 4) + 2;
        Indices[(i * 6) + 3] = (i * 4) + 2;
        Indices[(i * 6) + 4] = (i * 4) + 3;
        Indices[(i * 6) + 5] = (i * 4) + 0;
    }

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Textures_id[-1] = -1;
}

GRenderer::~GRenderer() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);

    glDeleteVertexArrays(1, &m_VAO);

    delete[] m_Buffer;
}


void GRenderer::Post_Event(const GEvent& Event) {
    std::unique_lock<std::recursive_mutex> Lck(m_Dispatcher_Mutex);
    if (Event.Renderer_Message == GERenderer_Message::Render)
        if (m_Render_Request < 2)
            m_Render_Request++;
        else
            return;

    auto Node = m_Queue.Get_Node();
    Node->Data = Event;
    m_Queue.Insert(Node);


    m_DCV.notify_all();
}


int GRenderer::Send_Event(const GEvent& Event) {
    {
        std::unique_lock<std::recursive_mutex> Lck(m_Dispatcher_Mutex);
        m_SEEvent = Event;

        GEvent Event;
        Event.Renderer_Message = GERenderer_Message::Send_Event;
        Post_Event(Event);
    }

    {
        std::unique_lock<std::recursive_mutex> Lck(m_SERM);
        m_SECV.wait(Lck, [=] { return m_SE_Continue.load(); });

        m_SE_Continue = false;
    }

    return m_SE_Ret;
}


int GRenderer::Send_Event_NL(const GEvent& Event) {
    std::unique_lock<std::recursive_mutex> Lck(m_Dispatcher_Mutex);
    return Callback_Func(Event);
}


void GRenderer::Start_Thread() {
    m_Worker = std::thread(&GRenderer::Worker, this);
}

void GRenderer::Join_Thread() {
    GEvent Event;
    Event.Renderer_Message = GERenderer_Message::Terminate_Thread;
    Post_Event(Event);

    m_Worker.join();
}


void GRenderer::Worker() {
    glfwMakeContextCurrent(m_Window_Hndl);

    while (m_Running) {
        std::unique_lock<std::recursive_mutex> Lck(m_Dispatcher_Mutex);
        m_DCV.wait(Lck, [=] { return !m_Queue.Empty(); });

        while (!m_Queue.Empty()) {
            GEvent Event = m_Queue.Peek_Front();
            Callback_Func(Event);

            if (Event.Renderer_Message == GERenderer_Message::Terminate_Thread)
                m_Running = false;

            m_Queue.Pop();
        }
    }

    glfwMakeContextCurrent(0);
}


int GRenderer::Callback_Func(const GEvent& Event) {
    switch (Event.Renderer_Message) {
        case GERenderer_Message::Render:
        {
            m_Render_Request--;
            Clear();

            GEvent Event;
            Event.Type = GEType::Window;
            Event.Wind_Message = GEWind_Message::Render;
            GCall(m_Main_Wind, m_Dispatcher_Ptr, Event);

            glfwSwapBuffers(m_Window_Hndl);
            break;
        }

        case GERenderer_Message::Load_Texture:
        {
            GTexture Texture = Event.Texture;
            glGenTextures(1, &Texture.ID);
            glBindTexture(GL_TEXTURE_2D, Texture.ID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            switch (Texture.Channels) {
            case 4:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Texture.Width, Texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Event.Data_Ptr);
                break;
            case 3:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Texture.Width, Texture.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Event.Data_Ptr);
                break;
            case 1:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Texture.Width, Texture.Height, 0, GL_RED, GL_UNSIGNED_BYTE, Event.Data_Ptr);
                break;
            }

            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

            return Texture.ID;
        }

        case GERenderer_Message::Send_Event:
        {
            m_SE_Ret = Callback_Func(m_SEEvent);
            m_SE_Continue = true;
            m_SECV.notify_all();

            break;
        }
    }

    return 0;
}



unsigned int GRenderer::Get_Shader() {
    return m_Shader;
}


void GRenderer::Set_Scale(const GSize& Window) {
    glUniform2f(m_Scale, (float)Window.X, (float)Window.Y);
}

void GRenderer::Set_Window_Space(const GPos& Screen, const GSize& Window) {
    glViewport(Screen.X, Screen.Y, Window.X, Window.Y);
    Set_Scale(Window);
}



void GRenderer::Add_Quad(const GQuad& Quad) {
    //Switching textures - if the texture is set
    if (Quad.m_Texture != -1) {
        if (Textures_id.find(Quad.m_Texture) == Textures_id.end()) {
            //Flush the buffer if more than 16 textures are used
            if (Current_Tex_id == 16) Render();

            Textures_id[Quad.m_Texture] = Current_Tex_id;
            Current_Tex_id++;
        }
    }

    //Copy the vertices of quad to memory
    Quad.Insert_Vertices(&m_Buffer[Quad_i * 4], Textures_id[Quad.m_Texture]);
    if (++Quad_i == __Quad_Count) Flush();
}


void GRenderer::Flush() { Render(); }
void GRenderer::Render() {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    if (Textures_id.size() > 1)
        for (const auto& Texture : Textures_id) {
            glActiveTexture(GL_TEXTURE0 + Texture.second);
            glBindTexture(GL_TEXTURE_2D, Texture.first);
        }

    glBufferSubData(GL_ARRAY_BUFFER, 0, Quad_i * 4 * sizeof(GVertex), m_Buffer);
    glDrawElements(GL_TRIANGLES, (GLsizei)Quad_i * 6, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Quad_i = 0;
    Current_Tex_id = 0;
    Textures_id.clear();
    Textures_id[-1] = -1;
}


void GRenderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}



#define Atlas_Size_X 2048
#define Atlas_Size_Y 2048

void GRenderer::Draw_Text(const GText& Str, const GPos& Pos, int Height) {
    if (!m_Main_Wind->Get_Default_Font()) {
        GError() << "Draw_Text called on no default font";
        return;
    }

    Draw_Text(Str, Pos, Height, m_Main_Wind->Get_Default_Font());
}

void GRenderer::Draw_Text(const GText& Str, const GPos& Pos, int Height, GFont* Font) {
    if (!Font) {
        GError() << "Draw_Text called with no font (nullptr)";
        return;
    }

    float Scale = (float)Height / (float)Font->Height;
    int X_Offset = 0;
    for (const auto& Ch : Str) {
        GAtlas& Atlas = Get_Atlas(Font, Ch.Code_Point);  //Generate if needed
        GAChar_Data& Ch_Data = Atlas.Map[Ch.Code_Point];

        //Remove float to int conversion data loss warning
        GQuad Quad(Ch_Data.Size.X * Scale, Ch_Data.Size.Y * Scale,
                   Pos.X + ((Ch_Data.Bearing.X + X_Offset) * Scale),
                   Pos.Y + ((-Ch_Data.Size.Y + Ch_Data.Bearing.Y + Font->Descender) * Scale));

        Quad.m_Type = GQuad_Type::Char;
        Quad.m_Color = Ch.Color;
        Quad.Texture_Region(Atlas.FB->Get_Texture(), Ch_Data.Size, Ch_Data.Pos);

        Add_Quad(Quad);
        X_Offset += Ch_Data.Advance;
    }
}


#include <chrono>

GAtlas& GRenderer::Get_Atlas(GFont* Font, unsigned int Code_Point) {
    for (auto& Atlas : Font->Atlas_List) {
        if (Atlas.Min <= Code_Point && Code_Point <= Atlas.Max) {
            if (Atlas.FB) return Atlas;

            //uint64_t Start = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            Fill_Atlas(Font, Atlas);
            //uint64_t End = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            //std::cout << "Atlas creation: " << End - Start << std::endl;


            return Atlas;
        }
    }

    GWarning() << "Couldn't find atlas for specified font";
    return Get_Empty_Atlas();
}

static inline void Set_Pixel(const GPos& Pos, unsigned char Color, unsigned char* Map) {
    int Offset = (Atlas_Size_X * Pos.Y + Pos.X);

    Map[Offset] = Color;
}


void GRenderer::Fill_Atlas(GFont* Font, GAtlas& Atlas) {
    unsigned char* Texture_Plane = new unsigned char[Atlas_Size_X * Atlas_Size_Y];
                                                    //Size * 4 channels (RGBW)
    memset(Texture_Plane, 0, Atlas_Size_X * Atlas_Size_Y);

    int X_Offset = 0, Y_Offset = 0;
    unsigned int Ch = 0;

    unsigned int Index = 0;
    if (Atlas.Min != FT_Get_First_Char(Font->Face, &Index)) GError() << "Atlas.Min != FT_Get_First_Char(Font->Face, &Index)";

    Ch = Atlas.Min;
    while (Ch <= Atlas.Max) {
        GAChar_Data Ch_Data;
        if (FT_Load_Char(Font->Face, Ch, FT_LOAD_RENDER)) {
            GError() << "FreeType: Failed to load Glyph";
        }

        auto& Glyph = Font->Face->glyph;

        Ch_Data.Size = { (int)Glyph->bitmap.width, (int)Glyph->bitmap.rows };
        Ch_Data.Bearing = { Glyph->bitmap_left,       Glyph->bitmap_top };
        Ch_Data.Advance = Glyph->advance.x >> 6;


        if (X_Offset + Ch_Data.Advance >= Atlas_Size_X) {
            X_Offset = 0;
            Y_Offset += Font->Height;
        }

        Ch_Data.Pos = { X_Offset + Ch_Data.Bearing.X, Y_Offset + Ch_Data.Bearing.Y + Font->Descender - Ch_Data.Size.Y };
        const GPos& Pos = Ch_Data.Pos;

        for (int Y = 0; Y < Ch_Data.Size.Y; Y++) {
            for (int X = 0; X < Ch_Data.Size.X; X++) {
                int Offset = (Ch_Data.Size.Y - Y - 1) * Ch_Data.Size.X + X;
                Set_Pixel({ Pos.X + X, Pos.Y + Y }, Glyph->bitmap.buffer[Offset], Texture_Plane);
            }
        }

        Atlas.Map[Ch] = Ch_Data;
        X_Offset += Ch_Data.Advance;

        Ch = FT_Get_Next_Char(Font->Face, Ch, &Index);
    }

    Set_Window_Space({ 0, 0 }, { Atlas_Size_X, Atlas_Size_Y });
    Atlas.FB = new GBasic_Framebuffer(Atlas_Size_X, Atlas_Size_Y);
    Atlas.FB->Bind();
    
    GTexture Texture;
    Texture.Channels = 1;
    Texture.Height = Atlas_Size_Y;
    Texture.Width = Atlas_Size_X;
    
    
    GEvent Event;
    Event.Renderer_Message = GERenderer_Message::Load_Texture;
    Event.Data_Ptr = Texture_Plane;
    Event.Texture = Texture;
    
    GQuad Quad(Atlas_Size_X, Atlas_Size_Y, 0, 0);
    Quad.m_Texture = Send_Event_NL(Event);
    Quad.m_Type = GQuad_Type::Atlas;
    
    Add_Quad(Quad);
    Flush();
    
    Atlas.FB->Un_Bind(m_Main_Wind->Get_Last_FB());
    
    glDeleteTextures(1, &Quad.m_Texture);
    delete[] Texture_Plane;
}


GAtlas& GRenderer::Get_Empty_Atlas() {
    static GAtlas Atlas;
    static GBasic_Framebuffer FB(1, 1);
    Atlas.FB = &FB;

    return Atlas;
}


unsigned int GRenderer::Store_Texture(unsigned char* Data, const GTexture& Texture) {
    GEvent Event;
    Event.Renderer_Message = GERenderer_Message::Load_Texture;
    Event.Data_Ptr = Data;
    Event.Texture = Texture;

    return Send_Event(Event);
}





static unsigned int Compile_Shader(unsigned int Type, const std::string& Src) {
    unsigned int id = glCreateShader(Type);
    const char* _src = Src.c_str();
    glShaderSource(id, 1, &_src, nullptr);
    glCompileShader(id);

    int Result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &Result);
    if (Result == GL_FALSE) {
        int Length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &Length);

    #ifdef _WIN64
        char* Message = (char*)_malloca(Length * sizeof(char));
    #else
        char* Message = (char*)alloca(Length * sizeof(char));
    #endif

        glGetShaderInfoLog(id, Length, &Length, Message);
        std::cout << "[Error::Compile_Shader] " << Message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Create_Shader(const std::string& Vertex, const std::string& Fragment) {
    unsigned int Program = glCreateProgram();
    unsigned int vs = Compile_Shader(GL_VERTEX_SHADER, Vertex);
    unsigned int fs = Compile_Shader(GL_FRAGMENT_SHADER, Fragment);

    //Linking stage
    glAttachShader(Program, vs);
    glAttachShader(Program, fs);

    glLinkProgram(Program);

    int Result;
    glGetProgramiv(Program, GL_LINK_STATUS, &Result);
    if (Result == GL_FALSE) {
        int Length;
        glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &Length);

    #ifdef _WIN64
        char* Message = (char*)_malloca(Length * sizeof(char));
    #else
        char* Message = (char*)alloca(Length * sizeof(char));
    #endif

        glGetProgramInfoLog(Program, Length, &Length, Message);
        std::cout << "[Error::Link_Program] " << Message << std::endl;

        glDeleteProgram(Program);
        return 0;
    }


    glValidateProgram(Program);

    //A bit of cleaning
    glDeleteShader(vs);
    glDeleteShader(fs);

    glUseProgram(Program);
    return Program;
}
