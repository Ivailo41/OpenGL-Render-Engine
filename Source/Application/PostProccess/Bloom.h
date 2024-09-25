#pragma once
#include "../GL_Objects/FrameBuffer.h"
#include "../GL_Objects/FrameQuad.h"
#include "../Shader.h"

class Bloom
{
public:
	unsigned getSteps() const { return steps; }

	//void applyEffect(const FrameBuffer& firstPass, const FrameBuffer& sceneBuffer);
	GLuint applyEffect(const FrameBuffer& firstPass, const FrameBuffer& sceneBuffer);

	void setSteps(unsigned steps);

public:
	Bloom(const Shader& blurShader, const Shader& bloomShader);

private:
	unsigned steps;

	FrameBuffer pp1, pp2;
	const Shader& blurShader;
	const Shader& bloomShader;

	static const unsigned MAX_STEPS = 100;
};
