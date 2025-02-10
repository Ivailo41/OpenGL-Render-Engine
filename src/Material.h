#pragma once
#include "Texture.h"
#include <GL/glew.h>
#include <gtc/type_ptr.hpp>
#include <vector>

class Material
{
public:

	void setTexture(Texture* texture, unsigned index);

	Texture* operator[](unsigned index) { return textures[index]; }
	const Texture* operator[](unsigned index) const { return textures[index]; }

	//void setName(const std::string& name);

	unsigned getShaderProgram() const { return shaderProgram; }
	void sendToShader() const;

	const std::string& getName() const { return name; }

	friend class FileManager;

	//TEMP
	void setRoughness(float rough)
	{
		roughness = rough;
		sendToShader();
	}

	void setMetalic(float metal)
	{
		metalic = metal;
		sendToShader();
	}

public:
	bool hasDiffuse;
	bool useNormalTexture;
	bool useORM;

private:
	Material();
	Material(const std::string& name);

private:
	static const char TEXTURES_COUNT = 3;

protected:
	std::string name;

	Texture* textures[TEXTURES_COUNT];
	unsigned shaderProgram;

	//Later make a derived class for PBR material
	glm::vec3 baseColor;
	float roughness;
	float metalic;
};

