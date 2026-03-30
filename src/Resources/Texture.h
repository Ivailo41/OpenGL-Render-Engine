#pragma once
#include <string>
#include <vector>
#include <iostream>

class Texture
{
public:
	unsigned textureId;
	std::string type; // can be enum if needed later on for something
	std::string path; //might be usefull to open the texture location through the software or reloading the texture

	//Casting the texture to unsigned will return the id, makes it easier to read instead in some functions
	operator unsigned() const;

	//Constructor to load texture from path
	Texture(const char* path);
	//Constructor to generate texture with already loaded data
	Texture(unsigned char* data, int width, int height, const char* path);

};