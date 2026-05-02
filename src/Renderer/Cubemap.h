#pragma once
#include <GL/glew.h>
#include <string>
#include "../Core/Logger.h"

enum class CubemapType
{
	SKYBOX,
	SHADOW_MAP,
	UNDEFINED = -1
};

class Cubemap
{
public:
	const GLuint getTextureID() const { return textureID; }
	void bind() const;

	//this function will be static because we cant have 2 bound textures at once and we unbind the only cubemap slot
	static void unbind();

	operator unsigned() const;

public:
	Cubemap();
	//Constructor to load cubemap
	Cubemap(const std::string texturePaths[6]);

	void generateCubemap(unsigned width, unsigned height, CubemapType type);

	~Cubemap();

private:
	GLuint textureID;
	CubemapType type;
};