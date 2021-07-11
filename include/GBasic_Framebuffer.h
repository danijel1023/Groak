#pragma once
#include "GQuad.h"

class GBasic_Framebuffer {
public:
    GBasic_Framebuffer(int Window_X, int Window_Y);
    ~GBasic_Framebuffer();


    void Bind();
    void Un_Bind(unsigned int FB);

    inline unsigned int Get_Texture() { return m_FB_Texture; }

private:
    GQuad m_FB_Quad;
    unsigned int m_Framebuffer = 0, m_FB_Texture = 0;
};
