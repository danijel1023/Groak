#include "GBasic_Framebuffer.h"
#include "GCore.h"

GBasic_Framebuffer::GBasic_Framebuffer(int Window_X, int Window_Y) {
    m_FB_Quad.m_Window.X = Window_X;
    m_FB_Quad.m_Window.Y = Window_Y;

    glGenFramebuffers(1, &m_Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

    glGenTextures(1, &m_FB_Texture);
    glBindTexture(GL_TEXTURE_2D, m_FB_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window_X, Window_Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FB_Texture, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_FB_Quad.m_Texture = m_FB_Texture;
}

GBasic_Framebuffer::~GBasic_Framebuffer() {
    glDeleteTextures(1, &m_FB_Texture);
    glDeleteFramebuffers(1, &m_Framebuffer);
}



void GBasic_Framebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
}

void GBasic_Framebuffer::Un_Bind(unsigned int FB) {
    glBindFramebuffer(GL_FRAMEBUFFER, FB);
}
