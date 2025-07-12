#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned getShaderProgram() const { return shaderProgram; }

public:
	//Shader(const std::string& vertexSource, const std::string& fragmentSource);
	//Shader(const unsigned vertexShader, const unsigned fragmentShader);

	void use() const;
	void deactivate() const;

	operator unsigned() const; //get the shader program by casting it to unsigned

	//returns location of the parameter
	GLuint setBool(const char* paramName, bool value) const;
	GLint setInt(const char* paramName, int value) const;
	GLint setFloat(const char* paramName, float value) const;
	GLuint setMat4(const char* paramName, const glm::mat4& value) const;
	GLuint setVec3(const char* paramName, const glm::vec3& value) const;


	//make this a map
	static std::unordered_map<std::string, Shader> shadersList;
	static const Shader* activeShader;
	static Shader* findShader(const std::string& shaderName);

	friend class FileManager;

protected:
	void createShaderProgram(const unsigned vertexShader, const unsigned fragmentShader,const unsigned geometryShader = 0);
	unsigned createShader(const std::string& shaderSource, const unsigned type);

	Shader();
	Shader(const std::string& shaderName, const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "");
	//Shader(const unsigned vertexShader, const unsigned fragmentShader);

	std::string name = "";
	unsigned shaderProgram = 0;
};

