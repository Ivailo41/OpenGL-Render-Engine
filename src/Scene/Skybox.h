#pragma once
#include "../Renderer/Cubemap.h"
#include "../Resources/Shader.h"
#include "../Renderer/Primitives.h"

class Skybox
{
public:
	void render() const;

public:
	Skybox(const std::string cubemapPath[6]);
	~Skybox();

private:
	const Cubemap cubemap;
	unsigned VAO, VBO;
};