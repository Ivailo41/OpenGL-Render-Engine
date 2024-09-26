#include "Cubemap.h"

void Cubemap::bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void Cubemap::unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Cubemap::Cubemap(GLuint texture) : textureID(texture)
{
	//nothing to do here
}
