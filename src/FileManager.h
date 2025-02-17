#pragma once
#include "Mesh.h"
#include <fstream>
#include <string>
#include "BaseObject.h"
#include <filesystem>
#include "Shader.h"
#include "Editor/Scene.h"

//for multithreading
#include <future>

//Might be better to rename it to ResourceManager since it will handle the recources instead of only opening files
class FileManager
{
public:
	//Can have System base class that will have start and stop functions, FileManager will inherit it
	static void init(); //initialises the FileManager system
	static void stop(); //stops the FileManager system

	bool loadOBJ(const std::string& fileName);
	void createDirectory(const std::string& path);


	//OLD FUNCTION
	GLuint loadTexture(const std::string& texturePath);

	//Using multithreading to load multiple textures faster
	void loadTextures(const std::vector<std::string>& texturesPaths);

	GLuint loadCubemap(const std::string texturePaths[6]);

	std::string loadShader(const std::string& shaderPath);
	bool loadShader(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragShaderPath);

	Material* const getMaterial(const std::string& name);
	unsigned isMaterialInList(const std::string& name);
	Material* addMaterial(const std::string name);
	bool removeMaterial(const std::string name);

	FileManager() {}

public:
	FileManager(const FileManager& other) = delete;

private:
	//using assert to check if the system is running
	void checkRunState();
	void operator=(const FileManager& other) {}
	//dynamically allocates mesh by given vertices, indices, name and material
	Mesh* createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const std::string& name, Material* const currentMaterial);

private:
	static bool isRunning;


};

