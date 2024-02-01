#pragma once
#include "Mesh.h"
#include <fstream>
#include <string>
#include "Object.h"

class FileManager
{
public:
	static Object* readOBJ(const std::string fileName);

	static std::string loadShader(const std::string shaderPath, unsigned type);
};

