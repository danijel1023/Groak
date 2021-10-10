#pragma once
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include "GQuad.h"
#include "GCharacter.h"
#include "GTexture.h"

#define G_QUAD_COUNT 256
#define G_ATLAS_SIZE_X 2048
#define G_ATLAS_SIZE_Y 2048

class GBasic_Framebuffer;
class GWindow;

class GRenderer {
public:
    GRenderer(GWindow* Main_Wind, GLFWwindow* Window_Hndl);
    ~GRenderer();

    void Draw_Text(const GText& Str, const GPos& Pos, int Height);  //Use default font
    void Draw_Text(const GText& Str, const GPos& Pos, int Height, GFont* Font);

    GAtlas& Get_Atlas(GFont* Font, unsigned int Code_Point);
    GAtlas& Get_Empty_Atlas();
    void Fill_Atlas(GFont* Font, GAtlas& Atlas);
    
    unsigned int Store_Texture(unsigned char* Data, const GTexture& Texture);

    //Call to add a quad to render
    void Add_Quad(const GQuad& Quad);

    void Flush();   //Flush / render to the framebuffer
    void Render();  //Exactly the same as Flush() - just different name
    void Clear();

    void Post_Event(const GEvent& Event);
    int Send_Event(const GEvent& Event);
    int Send_Event_NL(const GEvent& Event);
    void Start_Thread();
    void Join_Thread();
    void Start_Rendering();

    void Set_Window_Space(const GPos& Screen, const GSize& Window);
    void Set_Scale(const GSize& Window);

    unsigned int Get_Shader();
    static const unsigned char* Get_Integrated_Font_File();
    static size_t Get_Integrated_Font_File_Size();

private:
    GWindow* m_Main_Wind;
    GLFWwindow* m_Window_Hndl = nullptr;

    bool m_Running = true;
    GTEQueue m_Queue;
    int m_Render_Request = 0;
    std::atomic<bool> m_Allow_Render = false;
    std::recursive_mutex m_Dispatcher_Mutex;
    std::condition_variable_any m_DCV;


    std::recursive_mutex m_SERM;
    std::condition_variable_any m_SECV;
    std::atomic<bool> m_SE_Continue = false;
    int m_SE_Ret = 0;
    GEvent m_SEEvent;


    std::thread m_Worker;
    void Worker();
    int Callback_Func(const GEvent& Event);



    unsigned int m_VAO = 0, m_EBO = 0, m_VBO = 0;
    unsigned int m_Shader = 0;
    GVertex* m_Buffer = nullptr;
    int m_Scale = 0;
    size_t Quad_i = 0;
    std::map<unsigned int, int> Textures_id;
    unsigned int Current_Tex_id = 0;
};
