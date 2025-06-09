#include "Texture.h"
#include <GL/glew.h>

#include <stb_image.h>

//Texture::Texture() : id(0) //set the id to the default one indicating no texture
//{
//	//nothing to do here
//}

Texture::Texture(const char* path)
{
	textureId = 0;
	int width, height, nrChannels;
	//stbi_load uses malloc to allocate memory
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if(data)
	{
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	//stbi_image_free is a call to free() function for freeing allocated memory
	//this pointer colud be deleted with delete() as delete additionally calls the destructor of the class after it frees the memory same as free()

	stbi_image_free(data);
}

Texture::Texture(unsigned char* data, int width, int height, const char* path) : path(path)
{
	textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::operator unsigned() const
{
	//If we try to get id from a pointer to Texture that is set to nullptr we will return 0 which is the invalid texture id
	if (this == nullptr)
	{
		return 0;
	}

	return textureId;
}
