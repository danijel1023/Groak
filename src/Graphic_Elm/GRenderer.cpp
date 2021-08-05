#include <string>
#include <iostream>
#include <map>
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


GRenderer::GRenderer(GWindow* Main_Wind)
    : m_Main_Wind(Main_Wind) {
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
    glVertexAttribIPointer(1, 2, GL_INT,             sizeof(GVertex), (void*)offsetof(GVertex, Pos));
    glVertexAttribIPointer(2, 2, GL_INT,             sizeof(GVertex), (void*)offsetof(GVertex, Centre));
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



unsigned int GRenderer::Get_Shader() {
    return m_Shader;
}



void GRenderer::Set_Scale(int X, int Y) {
    glUniform2f(m_Scale, (float)X, (float)Y);
}

void GRenderer::Set_Window_Space(int Screen_X, int Screen_Y, int Window_X, int Window_Y) {
    glViewport(Screen_X, Screen_Y, Window_X, Window_Y);
    Set_Scale(Window_X, Window_Y);
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

void GRenderer::Draw_Text(const GText& Str, const GPos& Pos) {
    if (!m_Main_Wind->Get_Default_Font()) {
        GError() << "Draw_Text called on no default font";
        return;
    }

    Draw_Text(Str, Pos, m_Main_Wind->Get_Default_Font());
}

void GRenderer::Draw_Text(const GText& Str, const GPos& Pos, GFont* Font) {
    if (!Font) {
        GError() << "Draw_Text called with no font (nullptr)";
        return;
    }

    float Scale = (float)Font->Scale_Height / (float)Font->Height;
    int X_Offset = 0;
    for (const auto& Ch : Str) {
        GAtlas& Atlas = Get_Atlas(Font, Ch.Code_Point);  //Generate if needed
        GAChar_Data& Ch_Data = Atlas.Map[Ch.Code_Point];

        GQuad Quad(Ch_Data.Size.X * Scale, Ch_Data.Size.Y * Scale,
                   Pos.X + (Ch_Data.Bearing.X + X_Offset) * Scale,
                   Pos.Y + (-Ch_Data.Size.Y + Ch_Data.Bearing.Y + Font->Descender) * Scale);

        Quad.m_Type = GQuad_Type::Char;
        Quad.m_Color = Ch.Color;
        Quad.Texture_Region(Atlas.FB->Get_Texture(), Ch_Data.Size, Ch_Data.Pos);

        Add_Quad(Quad);
        X_Offset += Ch_Data.Advance;
    }
}


GAtlas& GRenderer::Get_Atlas(GFont* Font, unsigned int Code_Point) {
    for (auto& Atlas : Font->Atlas_List) {
        if (Atlas.Min <= Code_Point && Code_Point <= Atlas.Max) {
            if (Atlas.FB) return Atlas;

            Fill_Atlas(Font, Atlas);
            return Atlas;
        }
    }

    GWarning() << "Couldn't find atlas for specified font";
    return Get_Empty_Atlas();
}


void GRenderer::Fill_Atlas(GFont* Font, GAtlas& Atlas) {
    Set_Window_Space(0, 0, Atlas_Size_X, Atlas_Size_Y);
    GBasic_Framebuffer* FB = new GBasic_Framebuffer(Atlas_Size_X, Atlas_Size_Y);
    Atlas.FB = FB;
    FB->Bind();

    unsigned int Textures[16], Tex_I = 0;
    int X_Offset = 0, Y_Offset = 0;
    unsigned int Ch = 0;

    unsigned int Index = 0;
    if (Atlas.Min != FT_Get_First_Char(Font->Face, &Index)) GError() << "SHIUWHSLFJANC ?!?!!?";

    Ch = Atlas.Min;
    glGenTextures(16, Textures);
    while (Ch <= Atlas.Max) {
        GAChar_Data Ch_Data;
        if (FT_Load_Char(Font->Face, Ch, FT_LOAD_RENDER)) {
            GError() << "FreeType: Failed to load Glyph";
        }

        auto& Glyph = Font->Face->glyph;

        glBindTexture(GL_TEXTURE_2D, Textures[Tex_I]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     Glyph->bitmap.width,
                     Glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE,
                     Glyph->bitmap.buffer
        );

        Ch_Data.Size =    { (int)Glyph->bitmap.width, (int)Glyph->bitmap.rows };
        Ch_Data.Bearing = {      Glyph->bitmap_left,       Glyph->bitmap_top };
        Ch_Data.Advance =        Glyph->advance.x >> 6;


        if (X_Offset + Ch_Data.Advance >= 2048) {
            X_Offset = 0;
            Y_Offset += Font->Height;
        }


        GQuad Quad(Ch_Data.Size.X, Ch_Data.Size.Y,
                   X_Offset + Ch_Data.Bearing.X,
                   Y_Offset + Ch_Data.Bearing.Y + Font->Descender - Ch_Data.Size.Y);
        Quad.m_Type = GQuad_Type::Atlas;
        Quad.m_Texture = Textures[Tex_I];
        Quad.m_Color = { 1.0, 1.0, 1.0, 1.0 };
        Add_Quad(Quad);


        Ch_Data.Pos = Quad.m_Screen;
        Atlas.Map[Ch] = Ch_Data;
        X_Offset += Ch_Data.Advance;

        Ch = FT_Get_Next_Char(Font->Face, Ch, &Index);
        if (++Tex_I == 16) { Tex_I = 0; Flush(); }
        //It's using the same 16 textures, so it is important to manually flush
        //The automatic flush will occur only if 17 or more textures are in use
    }

    Flush();
    glDeleteTextures(16, Textures);
    FB->Un_Bind(m_Main_Wind->Get_Last_FB());
}


GAtlas& GRenderer::Get_Empty_Atlas() {
    static GAtlas Atlas;
    static GBasic_Framebuffer FB(1, 1);
    Atlas.FB = &FB;

    return Atlas;
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
