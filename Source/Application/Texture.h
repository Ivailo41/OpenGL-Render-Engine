#pragma once
#include <string>
#include <vector>
#include <iostream>

class Texture
{
public:
	static void loadTexture(const char* texturePath);

public:
	unsigned id;
	std::string type;
	std::string path;

	static std::vector<Texture> textures;
};