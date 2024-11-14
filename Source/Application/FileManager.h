#pragma once
#include "Mesh.h"
#include <fstream>
#include <string>
#include "Object.h"
#include <filesystem>
#include "Shader.h"

//for multithreading
#include <future>

class FileManager //can be an object that will have init() func
{
public:
	static void init(); //initialises the FileManager system
	static void stop(); //stops the FileManager system

	Object* readOBJ(const std::string& fileName);
	void createDirectory(const std::string& path);

	GLuint loadTexture(const std::string& texturePath);

	//Using multithreading to load multiple textures faster
	std::vector<Texture> loadTextures(const std::vector<std::string>& texturesPaths);

	GLuint loadCubemap(const std::string texturePaths[6]);
	std::string loadShader(const std::string& shaderPath);
	bool loadShader(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragShaderPath);

	FileManager() {}

public:
	FileManager(const FileManager& other) = delete;

private:
	void checkRunState();
	void operator=(const FileManager& other) {}

private:
	static bool isRunning;
};

