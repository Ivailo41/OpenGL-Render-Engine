#pragma once
#include <GL/glew.h>
#include "FrameBuffer.h"
#include "../Renderer/Shader.h"

class FrameQuad
{
public:
    static void initFrameQuad(const Shader* frameQuadShader);
    static void bindQuadVAO();

    //Draw a frame quad, shader and texture should be bound previously
    static void drawQuad();

    //Draw the drawTexture to the passed frameBuffer
    static void drawFrameQuad(GLuint drawTexture, const FrameBuffer& frameBuffer, float gamma, float exposure);

public:
    static GLuint quadVAO, quadVBO;
    static bool initialised;

private:
    static float quadVertices[];
    static const Shader* frameQuadShader;
};