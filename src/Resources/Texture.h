#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

class Texture
{
public:
	unsigned textureId;
	std::string type; // can be enum if needed later on for something
	std::filesystem::path path; //might be usefull to open the texture location through the software or reloading the texture

	//Casting the texture to unsigned will return the id, makes it easier to read instead in some functions
	operator unsigned() const;

	//Constructor to load texture from path
	Texture(const std::filesystem::path& path);
	//Constructor to generate texture with already loaded data
	Texture(unsigned char* data, int width, int height, const std::filesystem::path& path);

};