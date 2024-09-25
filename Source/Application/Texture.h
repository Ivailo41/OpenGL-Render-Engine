#pragma once
#include <string>
#include <vector>
#include <iostream>

class Texture
{
public:
	static void loadTexture(const char* texturePath); //put that inside the file manager and make this function take a data pointer to create a texture

	Texture();
	Texture(unsigned id);

public:
	unsigned id;
	std::string type; // can be enum if needed later on for something
	std::string path; //might be usefull to open the texture location through the software or reloading the texture

	//Move this container to the scene, so each scene will have different loaded textures.
	static std::vector<Texture> textures;
};