#pragma once
#include "../Scene/Scene.h"
#include "../Scene/Skybox.h"
#include "../Core/Window.h"
#include "../Core/Logger.h"
#include "FrameBuffer.h"
#include "../Resources/Shader.h"
#include "DebugShapes.h"
#include "FrameQuad.h"
#include "../glm/gtc/type_ptr.hpp"

class Renderer
{
public:
	Renderer();

	bool init(Window* window);
	void stop();

	bool isRunning() const { return running; }

	void onWindowResize(int width, int height);

	void renderScene(Scene* scene, Window* window);

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

public:
	static uint16_t SHADOW_WIDTH;
	static uint16_t SHADOW_HEIGHT;

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
	bool running = false;

	//Temp
	Cubemap shadowCubemap[4];
	glm::mat4 shadowMatrices[4][6];
};