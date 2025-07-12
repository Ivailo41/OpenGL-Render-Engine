#include "FrameBuffer.h"

void FrameBuffer::genFrameBuffer(unsigned width, unsigned height)
{
	if (frameBufferID != 0)
    {
		// If the framebuffer already exists, delete it
		deleteFrameBuffer();
    }

    this->width = width;
    this->height = height;

    glGenFramebuffers(1, &frameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

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
    if (hasDepth)
    {
        glGenRenderbuffers(1, &depthBufferID);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {}
        // log message when logging class is complete
}

void FrameBuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::FrameBuffer(unsigned buffersCount, bool depth) : buffersCount(buffersCount), width(0), height(0), hasDepth(depth),
frameBufferID(0), depthBufferID(0)
{
    //Nothing to do here
}

FrameBuffer::~FrameBuffer()
{
    if (!buffersCount == 0) 
    {
        glDeleteTextures(buffersCount, &buffers[0]);
    }

    deleteFrameBuffer();
}

void FrameBuffer::deleteFrameBuffer()
{
    glDeleteFramebuffers(1, &frameBufferID);
}

void ShadowFrameBuffer::genFrameBuffer(unsigned width, unsigned height)
{
    glGenFramebuffers(1, &frameBufferID);
    bind();

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
    {
        // Log or handle error
    }
}

void ShadowFrameBuffer::attachCubemap(const Cubemap& cubemap)
{
    depthBufferID = cubemap;
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBufferID, 0);
}

ShadowFrameBuffer::ShadowFrameBuffer() : FrameBuffer(0, false)
{

}
