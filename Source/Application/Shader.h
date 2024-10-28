#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader
{
public:
	unsigned getShaderProgram() const { return shaderProgram; }

public:
	Shader(const std::string& vertexSource, const std::string& fragmentSource);
	Shader(const unsigned vertexShader, const unsigned fragmentShader);

	void use() const;
	void deactivate() const;

	operator unsigned() const; //get the shader program by casting it to unsigned

	//returns location of the parameter
	GLint setInt(const char* paramName, int value) const;
	GLint setFloat(const char* paramName, float value) const;
	GLuint setMat4(const char* paramName, glm::mat4 value) const;

	static std::vector<Shader> shaders;
	static const Shader* activeShader;

protected:
	void createShaderProgram(const unsigned vertexShader, const unsigned fragmentShader);
	unsigned createShader(const std::string& shaderSource, const unsigned type);

private:
	unsigned shaderProgram;
};

