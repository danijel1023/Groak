#include "GCore.h"
#include "GBasic_Framebuffer.h"

GBasic_Framebuffer::GBasic_Framebuffer(const GSize& Window) {
    m_FB_Texture.Size = Window;

    glGenFramebuffers(1, &m_Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);

    glGenTextures(1, &m_FB_Texture.ID);
    glBindTexture(GL_TEXTURE_2D, m_FB_Texture.ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window.X, Window.Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FB_Texture.ID, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GBasic_Framebuffer::~GBasic_Framebuffer() {
    glDeleteTextures(1, &m_FB_Texture.ID);
    glDeleteFramebuffers(1, &m_Framebuffer);
}



void GBasic_Framebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
}

void GBasic_Framebuffer::Un_Bind(unsigned int FB) {
    glBindFramebuffer(GL_FRAMEBUFFER, FB);
}
