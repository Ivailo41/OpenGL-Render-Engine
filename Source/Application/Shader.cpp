#include "Shader.h"

std::vector<unsigned> Shader::shaders;

Shader::Shader(const std::string vertexPath, const std::string fragmentPath) : shaderProgram(0)
{
	std::string vertexSource = FileManager::loadShader(vertexPath, GL_VERTEX_SHADER);
	std::string fragmentSource = FileManager::loadShader(fragmentPath, GL_FRAGMENT_SHADER);

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

unsigned Shader::createShader(const std::string shaderSource, const unsigned type)
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
	shaders.push_back(shaderProgram);
}
