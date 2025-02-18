#pragma once
#include "../Renderer/Cubemap.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Primitives.h"
#include "../Scene/Camera.h"

class Skybox
{
public:
	void render(const Camera* camera) const;

public:
	Skybox(const Cubemap* cubemap, const Shader* skyboxShader);
	~Skybox();

private:
	const Cubemap* cubemap;
	const Shader* skyboxShader;
	unsigned VAO, VBO;
};