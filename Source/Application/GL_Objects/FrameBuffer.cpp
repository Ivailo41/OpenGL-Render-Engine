#include "FrameBuffer.h"

void FrameBuffer::genFrameBuffer(unsigned width, unsigned height)
{
    deleteFrameBuffer();

    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    for (size_t i = 0; i < buffersCount; i++)
    {
        buffers.push_back(0);
    }

    //generate color buffers
    glGenTextures(buffersCount, &buffers[0]);
    for (unsigned int i = 0; i < buffersCount; i++)
    {
        glBindTexture(GL_TEXTURE_2D, buffers[i]);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, buffers[i], 0
        );
    }

    //generate depth buffer if we need it
    if(hasDepth)
    {
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer(unsigned buffersCount, bool depth) : buffersCount(buffersCount), width(0), height(0), hasDepth(depth),
                                                              fbo(0), rbo(0)
{
    //Nothing to do here
}

FrameBuffer::~FrameBuffer()
{
    glDeleteTextures(buffersCount, &buffers[0]);
    glDeleteFramebuffers(1, &fbo);
}

void FrameBuffer::deleteFrameBuffer()
{
    glDeleteFramebuffers(1, &fbo);
}


