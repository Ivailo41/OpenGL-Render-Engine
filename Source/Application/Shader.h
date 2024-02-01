#pragma once
#include "FileManager.h"
#include <string>

class Shader
{
public:
	unsigned getShaderProgram() const { return shaderProgram; }

public:
	Shader(const std::string vertexPath, const std::string fragmentPath);
	Shader(const unsigned vertexShader, const unsigned fragmentShader);

	static std::vector<unsigned> shaders;

protected:
	void createShaderProgram(const unsigned vertexShader, const unsigned fragmentShader);
	unsigned createShader(const std::string shaderSource, const unsigned type);

private:
	unsigned shaderProgram;
};

