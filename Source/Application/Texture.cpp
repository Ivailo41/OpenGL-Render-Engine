#include "Texture.h"
#include <GL/glew.h>

std::vector<Texture*> Texture::textures; //make that unordered set to prevent repetition

Texture::Texture() : id(0) //set the id to the default one indicating no texture
{

}

Texture::Texture(unsigned id) : id(id)
{
	textures.push_back(this);
}
