#pragma once
#include "Texture.h"
#include <GL/glew.h>
#include "glm/gtc/type_ptr.hpp"
#include <vector>

class Material
{
public:

	void setTexture(const Texture& texture, unsigned index);

	Texture& operator[](unsigned index) { return textures[index]; }
	const Texture& operator[](unsigned index) const { return textures[index]; }

	void setName(const std::string& name);

	unsigned getShaderProgram() const { return shaderProgram; }
	void sendToShader() const;

	static unsigned isMaterialInList(const std::string& name);

	static Material* const getMaterial(const std::string& name);
	static Material* const getMaterial(unsigned index);

	static unsigned getMaterialsCount() { return materials.size(); }

	static Material* addMaterial(const std::string name);
	static bool removeMaterial(const std::string name);

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
	Material();
	Material(const std::string& name);

private:
	static const char TEXTURES_COUNT = 3;

protected:
	std::string name;

	//make this pointers to textures
	Texture textures[TEXTURES_COUNT];
	unsigned shaderProgram;

	//Later make a derived class for PBR material
	glm::vec3 baseColor;
	float roughness;
	float metalic;

	bool hasDiffuse;
	bool useNormalTexture;
	bool useORM;

	//Move this container to the scene, so each scene will have different loaded materials.
	static std::vector<Material*> materials;

};

