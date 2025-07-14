#pragma once
#include "../Scene/Mesh.h"
#include <fstream>
#include <string>
#include "../Scene/BaseObject.h"
#include <filesystem>
#include "../Renderer/Shader.h"
#include "../Scene/Scene.h"
#include <sstream>

//for multithreading
#include <future>

//Might be better to rename it to ResourceManager since it will handle the recources instead of only opening files
class FileManager
{
public:
	//Can have System base class that will have start and stop functions, FileManager will inherit it
	bool init(); //initialises the FileManager system
	void stop(); //stops the FileManager system

	bool loadOBJ(const std::string& fileName);
	void createDirectory(const std::string& path);

	//Function that checks the file existance found from stackoverflow answer
	inline bool checkFileExistance(const std::string& name);

	//Using multithreading to load multiple textures faster
	void loadTextures(const std::vector<std::string>& texturesPaths);

	std::string loadShader(const std::string& shaderPath);
	bool loadShader(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragShaderPath, const std::string& geometryShader = "");

	//Might make these scene functions
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
	Mesh* createMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const std::string& name, const std::vector<MaterialGroup>& matGroups);

	unsigned FileManager::tokenizeOBJFaceLine(std::vector<std::string>& tokens, const std::string& line);

private:
	static bool isRunning;


};

