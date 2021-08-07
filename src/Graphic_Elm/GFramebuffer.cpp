#include "GCore.h"
#include "GFramebuffer.h"
#include "GRenderer.h"
#include "GWindow.h"

GFramebuffer::GFramebuffer(int Window_X, int Window_Y, GBasic_Window* Current_Window)
    : GFramebuffer({ Window_X, Window_Y }, Current_Window) {}

GFramebuffer::GFramebuffer(const GSize& Window, GBasic_Window* Current_Window) {
    m_Current_Window = Current_Window;
    m_FB_Quad.m_Window = Window;

    glGenFramebuffers(1, &m_Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

    glGenTextures(1, &m_FB_Texture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_FB_Texture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 32, GL_RGBA, Window.X, Window.Y, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_FB_Texture, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_FB_Quad.m_Texture = m_FB_Texture;
}

GFramebuffer::~GFramebuffer() {
    glDeleteTextures(1, &m_FB_Texture);
    glDeleteFramebuffers(1, &m_Framebuffer);
}



void GFramebuffer::Use() {
    m_Current_Window->m_Main_Window->m_FB_Ancor = { m_Current_Window->m_Absolute_Screen };

    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    m_Current_Window->m_Main_Window->m_Last_Framebuffer.push(this);
}


void GFramebuffer::Render() {
    auto& Main_Window = m_Current_Window->m_Main_Window;
    Main_Window->m_Last_Framebuffer.pop();
    auto& Previous_Framebuffer = Main_Window->m_Last_Framebuffer.top();
    auto& Renderer = *Main_Window->m_Renderer;

    if (Previous_Framebuffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, Previous_Framebuffer->m_Framebuffer);
        Main_Window->m_FB_Ancor = { Previous_Framebuffer->m_Current_Window->m_Absolute_Screen };
    }

    else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        Main_Window->m_FB_Ancor = { 0, 0 };
    }

    m_Current_Window->Set_Viewport();
    Renderer.Add_Quad(m_FB_Quad);
    Renderer.Flush();
}
