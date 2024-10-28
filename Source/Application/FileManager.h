#pragma once
#include "Mesh.h"
#include <fstream>
#include <string>
#include "Object.h"
#include <filesystem>
#include "Shader.h"

//for multithreading
#include <future>

class FileManager //can be an object
{
public:
	static Object* readOBJ(const std::string& fileName);
	static void createDirectory(const std::string& path);

	static GLuint loadTexture(const std::string& texturePath);

	//Using multithreading to load multiple textures faster
	static std::vector<Texture> loadTextures(const std::vector<std::string>& texturesPaths);

	static GLuint loadCubemap(const std::string texturePaths[6]);
	static std::string loadShader(const std::string& shaderPath);
};

