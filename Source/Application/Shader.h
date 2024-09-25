#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <GL/glew.h>

class Shader
{
public:
	unsigned getShaderProgram() const { return shaderProgram; }

public:
	Shader(const std::string vertexPath, const std::string fragmentPath);
	Shader(const unsigned vertexShader, const unsigned fragmentShader);

	void use() const;
	void deactivate() const;

	operator unsigned() const; //get the shader program by casting it to unsigned

	//returns location of the parameter
	GLint setInt(const char* paramName, int value) const;
	GLint setFloat(const char* paramName, float value) const;

	static std::vector<Shader> shaders;
	static const Shader* activeShader;

protected:
	void createShaderProgram(const unsigned vertexShader, const unsigned fragmentShader);
	unsigned createShader(const std::string shaderSource, const unsigned type);

	std::string loadShader(const std::string shaderPath, unsigned type);

private:
	unsigned shaderProgram;
};

