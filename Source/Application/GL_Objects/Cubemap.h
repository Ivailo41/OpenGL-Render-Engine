#pragma once
#include <GL/glew.h>

class Cubemap
{
public:
	const GLuint getTextureID() const { return textureID; }
	void bind() const;

	//this function will be static because we cant have 2 bound textures at once and we unbind the only cubemap slot
	static void unbind();

public:
	Cubemap(GLuint texture);

private:
	GLuint textureID;
};