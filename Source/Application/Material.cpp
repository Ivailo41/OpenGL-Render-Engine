#include "Material.h"
#include <iostream>
#include "Shader.h"

std::vector<Material*> Material::materials;

void Material::setTexture(const Texture& texture, unsigned index)
{
	switch (index)
	{
	case 0:
		hasDiffuse = true;
		break;
	case 1:
		useORM = true;
		break;
	case 2:
		useNormalTexture = true;
		break;
	default:
		break;
	}

	textures[index] = texture;
	sendToShader();
}

Material::Material() : baseColor(1,1,1), roughness(0.3), metalic(0),
						hasDiffuse(false), useNormalTexture(false), useORM(false)
{
	setName("New_Material");
	shaderProgram = Shader::shaders[0];
	sendToShader();
}

Material::Material(const std::string& name) : baseColor(1, 1, 1), roughness(0.3), metalic(0),
												hasDiffuse(false), useNormalTexture(false), useORM(false)
{
	setName(name);
	shaderProgram = Shader::shaders[0];
	sendToShader();
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
	if(index >= materials.size())
	{
		throw std::runtime_error("No material with that index");
	}
	return materials[index];
}

void Material::sendToShader() const
{
	GLint baseColorLoc = glGetUniformLocation(shaderProgram, "baseColor");
	GLint roughnessLoc = glGetUniformLocation(shaderProgram, "roughness");
	GLint metalicLoc = glGetUniformLocation(shaderProgram, "metalic");

	GLint hasDiffuseLoc = glGetUniformLocation(shaderProgram, "hasDiffuse");
	GLint useNormalTextureLoc = glGetUniformLocation(shaderProgram, "useNormalTexture");
	GLint useORMLoc = glGetUniformLocation(shaderProgram, "useORM");

	GLint baseImageLoc = glGetUniformLocation(shaderProgram, "diffTexture");
	GLint ORMImageLoc = glGetUniformLocation(shaderProgram, "ORMTexture");      // ORM = Occlusion, Roughness, Metallic
	GLint normalMapLoc = glGetUniformLocation(shaderProgram, "normalTexture");

	glUseProgram(shaderProgram);

	glUniform3f(baseColorLoc, baseColor.r, baseColor.g, baseColor.b);
	glUniform1f(roughnessLoc, roughness);
	glUniform1f(metalicLoc, metalic);

	glUniform1i(hasDiffuseLoc, hasDiffuse);
	glUniform1i(useNormalTextureLoc, useNormalTexture);
	glUniform1i(useORMLoc, useORM);

	glUniform1i(baseImageLoc, 0); // Texture unit 0 is for base images.
	glUniform1i(ORMImageLoc, 1); // Texture unit 1 is for ORM images.
	glUniform1i(normalMapLoc, 2); // Texture unit 2 is for normal maps.
}