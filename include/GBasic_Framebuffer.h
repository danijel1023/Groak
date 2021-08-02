#pragma once
#include "GTexture.h"

class GBasic_Framebuffer {
public:
    GBasic_Framebuffer(int Window_X, int Window_Y);
    ~GBasic_Framebuffer();

    void Bind();
    void Un_Bind(unsigned int FB);

    inline GTexture& Get_Texture() { return m_FB_Texture; }

private:
    unsigned int m_Framebuffer = 0;
    GTexture m_FB_Texture;
};
