#pragma once
#include "Mesh.h"
#include <fstream>
#include <string>
#include "Object.h"
#include <filesystem>

class FileManager
{
public:
	static Object* readOBJ(const std::string fileName);
	static void createDirectory(const std::string path);
	static std::string loadShader(const std::string shaderPath, unsigned type);
};

