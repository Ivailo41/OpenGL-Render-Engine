#include "Material.h"
#include <iostream>
#include "Shader.h"

std::vector<Material*> Material::materials;

void Material::setTexture(const Texture& texture, unsigned index)
{
	textures[index] = texture;
	sendToShader();
}

Material::Material()
{
	setName("New_Material");
	textures.push_back(Texture());
	textures.push_back(Texture());
	textures.push_back(Texture());
	shaderProgram = Shader::shaders[0];
}

Material::Material(const std::string& name)
{
	setName(name);
	textures.push_back(Texture());
	textures.push_back(Texture());
	textures.push_back(Texture());
	shaderProgram = Shader::shaders[0];
}

Material::Material(const Material& other)
{
	copyFrom(other);
}

Material& Material::operator=(const Material& other)
{
	if (this != &other)
	{
		textures.clear();
		copyFrom(other);
	}
	return *this;
}

void Material::copyFrom(const Material& other)
{
	setName(other.name);
	unsigned texturesCount = other.textures.size();
	for (size_t i = 0; i < texturesCount; i++)
	{
		textures.push_back(other.textures[i]);
	}
	shaderProgram = other.shaderProgram;
}

void Material::setName(const std::string& name)
{
	if(name.empty())
	{
		return;
	}
	if (isMaterialInList(name) != 0)
	{
		this->name += '_' + std::to_string(isMaterialInList(name));
		return;
	}
	this->name = name;
}

unsigned Material::isMaterialInList(const std::string& name)
{
	unsigned timesFound = 0;
	unsigned materialsCount = materials.size();
	for (size_t i = 0; i < materialsCount; i++)
	{
		if (materials[i]->name.substr(0, materials[i]->name.find_last_of('_') - 1) == name)
		{
			timesFound++;
		}
	}
	return timesFound;
}

Material* const Material::getMaterial(const std::string& name)
{
	unsigned materialsCount = materials.size();
	for (size_t i = 0; i < materialsCount; i++)
	{
		if (materials[i]->name == name)
		{
			return materials[i];
		}
	}
}

Material* Material::addMaterial(const std::string name)
{
	Material* material = new Material(name);
	materials.push_back(material);
	return material;
}

bool Material::removeMaterial(const std::string name)
{
	unsigned materialsCount = materials.size();
	for (size_t i = 0; i < materialsCount; i++)
	{
		if (materials[i]->name == name)
		{
			delete materials[i];
			materials.erase(materials.begin() + i);
			return true;
		}
	}
	return false;
}

Material* const Material::getMaterial(unsigned index)
{
	return materials[index];
}

void Material::sendToShader() const
{
	GLint baseImageLoc = glGetUniformLocation(shaderProgram, "diffTexture");
	GLint ORMImageLoc = glGetUniformLocation(shaderProgram, "ORMTexture");      // ORM = Occlusion, Roughness, Metallic
	GLint normalMapLoc = glGetUniformLocation(shaderProgram, "normalTexture");

	glUseProgram(shaderProgram);
	glUniform1i(baseImageLoc, 0); // Texture unit 0 is for base images.
	glUniform1i(ORMImageLoc, 1); // Texture unit 1 is for ORM images.
	glUniform1i(normalMapLoc, 2); // Texture unit 2 is for normal maps.
}