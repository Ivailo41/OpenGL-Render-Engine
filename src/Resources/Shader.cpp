#include "../Resources/Shader.h"
#include <iostream>

std::unordered_map<std::string, Shader> Shader::shadersList;
const Shader* Shader::activeShader = nullptr;

Shader::Shader()
{
	//default constructor
}

Shader::Shader(const std::string& shaderName, const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource) : shaderProgram(0), name(shaderName)
{
	if (vertexSource.size() == 0 || fragmentSource.size() == 0)
	{
		throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
	}
	try
	{
		unsigned int vertexShader = createShader(vertexSource, GL_VERTEX_SHADER);
		unsigned int fragmentShader = createShader(fragmentSource, GL_FRAGMENT_SHADER);
		unsigned int geometryShader = 0;

		if (geometrySource.size() > 0)
		{
			geometryShader = createShader(geometrySource, GL_GEOMETRY_SHADER);
		}

		createShaderProgram(vertexShader, fragmentShader, geometryShader);
	}
	catch (std::exception& e)
	{
		throw e;
	}

	Shader::shadersList.insert(std::pair(shaderName, *this));
}

void Shader::use() const
{
	glUseProgram(shaderProgram);
	activeShader = this;
}

void Shader::deactivate() const
{
	glUseProgram(0);
	activeShader = nullptr;
}

Shader::operator unsigned() const
{
	return shaderProgram;
}

GLuint Shader::setBool(const char* paramName, bool value) const
{
	GLuint location = glGetUniformLocation(shaderProgram, paramName);
	glUniform1i(location, (int)value); // OpenGL uses int for boolean uniforms
	return location;
}

GLint Shader::setInt(const char* paramName, int value) const
{
	GLint location = glGetUniformLocation(shaderProgram, paramName);
	glUniform1i(location, value);
	return location;
}

GLint Shader::setFloat(const char* paramName, float value) const
{
	GLint location = glGetUniformLocation(shaderProgram, paramName);
	glUniform1f(location, value);
	return location;
}

GLuint Shader::setMat4(const char* paramName, const glm::mat4& value) const
{
	GLuint location = glGetUniformLocation(shaderProgram, paramName);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	return location;
}

GLuint Shader::setVec3(const char* paramName, const glm::vec3& value) const
{
	GLuint location = glGetUniformLocation(shaderProgram, paramName);
	glUniform3fv(location, 1, glm::value_ptr(value));
	return location;
}

Shader* Shader::findShader(const std::string& shaderName)
{
	auto shaderItt = shadersList.find(shaderName);

	if(shaderItt == shadersList.end())
	{
		return nullptr;
	}

	return &shaderItt.operator*().second;
}

unsigned Shader::createShader(const std::string& shaderSource, const unsigned type)
{
	const char* shaderSourceChar = shaderSource.c_str();

	unsigned shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSourceChar, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		throw std::runtime_error("ERROR::SHADER::COMPILATION_FAILED\n");

		return 0;
	}

	return shader;
}

void Shader::createShaderProgram(const unsigned vertexShader, const unsigned fragmentShader, const unsigned geometryShader)
{
	unsigned shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	if (geometryShader != 0)
	{
		glAttachShader(shaderProgram, geometryShader);
	}

	glLinkProgram(shaderProgram);

	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	this->shaderProgram = shaderProgram;
}
