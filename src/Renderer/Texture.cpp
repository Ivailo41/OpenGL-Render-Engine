#include "Texture.h"
#include <GL/glew.h>

Texture::Texture() : id(0) //set the id to the default one indicating no texture
{
	//nothing to do here
}

Texture::Texture(unsigned id, const char* path) : id(id), path(path)
{
	//nothing to do here
}

Texture::operator unsigned() const
{
	//If we try to get id from a pointer to Texture that is set to nullptr we will return 0 which is the invalid texture id
	if (this == nullptr)
	{
		return 0;
	}

	return id;
}
