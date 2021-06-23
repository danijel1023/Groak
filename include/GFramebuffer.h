#pragma once
#include <stack>
#include "GCore.h"
#include "GQuad.h"

class GBasic_Window;
class GFrameBuffer {
public:
    GFrameBuffer(int Window_X, int Window_Y, GBasic_Window* Current_Window);
    ~GFrameBuffer();

    void Use();
    void Render();

private:
    GQuad m_FB_Quad;
    GBasic_Window* m_Current_Window = nullptr;

    unsigned int m_Framebuffer = 0, m_FB_Texture = 0;
};
