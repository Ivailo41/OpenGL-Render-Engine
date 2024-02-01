#pragma once
#include "Texture.h"
#include <GL/glew.h>
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

public:
	Material();
	Material(const std::string& name);
	Material(const Material& other);
	Material& operator=(const Material& other);
	~Material() = default;

protected:
	void copyFrom(const Material& other);

protected:
	std::string name;
	std::vector<Texture> textures;
	unsigned shaderProgram;

	static std::vector<Material*> materials;

};

