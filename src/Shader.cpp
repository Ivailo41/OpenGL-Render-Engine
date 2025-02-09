#include "Shader.h"

std::unordered_map<std::string, Shader> Shader::shadersList;
std::vector<Shader> Shader::shaders;
const Shader* Shader::activeShader = nullptr;

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) : shaderProgram(0)
{
	if (vertexSource.size() == 0 || fragmentSource.size() == 0)
	{
		throw std::exception("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
	}

	try
	{
		unsigned int vertexShader = createShader(vertexSource, GL_VERTEX_SHADER);
		unsigned int fragmentShader = createShader(fragmentSource, GL_FRAGMENT_SHADER);

		createShaderProgram(vertexShader, fragmentShader);
	}
	catch (std::exception& e)
	{
		throw e;
	}
}

Shader::Shader(const unsigned vertexShader, const unsigned fragmentShader) : shaderProgram(0)
{
	try
	{
		createShaderProgram(vertexShader, fragmentShader);
	}
	catch (std::exception& e)
	{
		throw e;
	}
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

GLuint Shader::setMat4(const char* paramName, glm::mat4 value) const
{
	GLuint location = glGetUniformLocation(shaderProgram, paramName);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	return location;
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

		throw std::exception("ERROR::SHADER::COMPILATION_FAILED\n");

		return 0;
	}

	return shader;
}

void Shader::createShaderProgram(const unsigned vertexShader, const unsigned fragmentShader)
{
	unsigned shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

		throw std::exception("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	this->shaderProgram = shaderProgram;
	shaders.push_back(*this);
}
