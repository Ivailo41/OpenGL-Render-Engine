#pragma once
#include "../Scene/Scene.h"
#include "../Scene/Skybox.h"
#include "../Scene/Lights/Light.h"
#include "../Scene/Lights/PointLight.h"
#include "../Core/Window.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "DebugShapes.h"
#include "FrameQuad.h"

class Renderer
{
public:
	Renderer();

	bool init(Window* window);
	void stop();

	void onWindowResize(int width, int height);

	void render(Scene* scene, Window* window);

	const FrameBuffer& getScreneFrameBuffer() const { return screneFrameBuffer; }

public:
	float bloomThreshold = 0.5f; //default threshold for bloom effect
	int bloomSteps = 10; //default number of steps for bloom effect
	bool isBloomEnabled = false; //default bloom effect state

	float gamma = 1.0f; //default gamma value
	float exposure = 1.0f; //default exposure value

	DebugShapes debugShapes;

	bool drawTangents = false; //default tangent drawing state
	float tangentLength = 0.1f; //default tangent length

protected:
	ShadowFrameBuffer shadowFrameBuffer;
	FrameBuffer colorFrameBuffer;
	FrameBuffer screneFrameBuffer;

	Shader* shadowShader;
	Shader* pbrShader;
	Shader* debugShader;
	Shader* tangentShader;
	Shader* frameQuadShader;
	Shader* skyboxShader;

	GLuint sceneTexture = 0; //texture for the scene
};