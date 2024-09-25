#pragma once
#include "Mesh.h"
#include <fstream>
#include <string>
#include "Object.h"
#include <filesystem>

#include "Shader.h"
//include shader here and use the file manager to return a shader instead of including the file manager header in the shader header
//which causes the shader to include the object.h and the baseobject.h which I dont want

class FileManager //can be an object
{
public:
	static Object* readOBJ(const std::string fileName);
	static void createDirectory(const std::string path);
};

