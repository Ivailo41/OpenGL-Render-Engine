#pragma once
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include "Logger.h"

#include "../Resources/Raw/RawStructs.h"

//for multithreading
#include <future>

//Might be better to rename it to ResourceManager since it will handle the resources instead of only opening files
class FileManager
{
public:
	//Can have System base class that will have start and stop functions, FileManager will inherit it
	bool init(); //initializes the FileManager system
	void stop(); //stops the FileManager system

	bool isRunning() const { return running; }

	RawModel loadOBJ(const std::filesystem::path& filePath) const;
	void createDirectory(const std::string& path) const;

	//Function that checks the file existence found from stackoverflow answer
	static inline bool checkFileExistence(const std::string& name);

	//Using multithreading to load multiple textures faster
	void loadTextures(const std::vector<std::string>& texturesPaths);

	std::vector<std::string> loadShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::filesystem::path& geometryPath) const;

	FileManager() = default;

public:
	FileManager(const FileManager& other) = delete;

private:
	void operator=(const FileManager& other) const {}

	static unsigned tokenizeOBJFaceLine(std::vector<char*>& tokens, char* line);

	static void getPrefixFromLine(char* line, char* prefix);
	bool running = false;
};

