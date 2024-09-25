#include "FrameQuad.h"

float FrameQuad::quadVertices[] = {
    // Positions        // Texture Coords
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};

bool FrameQuad::initialised = false;
const Shader* FrameQuad::frameQuadShader = nullptr;

GLuint FrameQuad::quadVAO = 0;
GLuint FrameQuad::quadVBO = 0;

void FrameQuad::initFrameQuad(const Shader* frameQuadShader)
{
    if(!initialised)
    {
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);

        glBindVertexArray(quadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Texture Coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        FrameQuad::frameQuadShader = frameQuadShader;

        initialised = true;
    }
}

void FrameQuad::bindQuadVAO()
{
    glBindVertexArray(quadVAO);
}

void FrameQuad::drawQuad()
{
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void FrameQuad::drawFrameQuad(GLuint drawTexture, const FrameBuffer& frameBuffer)
{
    frameBuffer.bind();

    frameQuadShader->use();
    frameQuadShader->setInt("scene", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, drawTexture);

    drawQuad();
}
