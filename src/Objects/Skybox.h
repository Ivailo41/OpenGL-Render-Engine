#pragma once
#include "../GL_Objects/Cubemap.h"
#include "../Shader.h"
#include "../Primitives.h"
#include "../Camera.h"

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