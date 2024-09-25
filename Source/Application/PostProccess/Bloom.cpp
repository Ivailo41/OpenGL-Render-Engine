#include "Bloom.h"
#include <iostream>

GLuint Bloom::applyEffect(const FrameBuffer& firstPass, const FrameBuffer& sceneBuffer)
{
	bool horizontal = true;
	bool first_iteration = true;

    FrameBuffer *pingpongFBO[] = { &pp1,&pp2 };
    blurShader.use();

    FrameQuad::bindQuadVAO();

    for (unsigned int i = 0; i < steps; i++)
    {
        pingpongFBO[horizontal]->bind();
        blurShader.setInt("horizontal", horizontal);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(
            GL_TEXTURE_2D, first_iteration ? firstPass[1] : pingpongFBO[!horizontal]->operator[](0)
            //fun fact, if you want to use overloaded operator[] on a pointer it should be called with full name or dereference the pointer
        );
        glDrawArrays(GL_TRIANGLES, 0, 6);

        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    bloomShader.use();
    //sceneBuffer.bind();
    pingpongFBO[horizontal]->bind();

    bloomShader.setInt("scene", 0);
    bloomShader.setInt("bloomBlur", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, firstPass[0]);

    glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, pingpongFBO[1]->operator[](0));
    glBindTexture(GL_TEXTURE_2D, pingpongFBO[!horizontal]->operator[](0));

    FrameQuad::drawQuad();
    glBindVertexArray(0);

    return pingpongFBO[horizontal]->operator[](0);
}

void Bloom::setSteps(unsigned steps)
{
    if (steps < MAX_STEPS)
        this->steps = steps;
    else
        this->steps = MAX_STEPS;
}

Bloom::Bloom(const Shader& blurShader, const Shader& bloomShader) : pp1(1, false), pp2(1, false), steps(10),
																	blurShader(blurShader), bloomShader(bloomShader)
{
	pp1.genFrameBuffer(1920, 1080);
	pp2.genFrameBuffer(1920, 1080);
}
