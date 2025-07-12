#pragma once
#include <GL/glew.h>

class FrameQuad
{
public:
    static void initFrameQuad();
    static void bindQuadVAO();

    //Draw a frame quad, shader and texture should be bound previously
    static void drawQuad();

    //Draw the drawTexture to the passed frameBuffer
    static void drawFrameQuad(GLuint drawTexture);

public:
    static GLuint quadVAO, quadVBO;
    static bool initialised;

private:
    static float quadVertices[];
};