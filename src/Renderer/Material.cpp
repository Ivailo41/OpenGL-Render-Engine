#include "Material.h"
#include <iostream>

void Material::setTexture(Texture* texture, unsigned index)
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

Material::Material() : baseColor(1,1,1), roughness(0.3), metalic(0), name("New_Material"),
						hasDiffuse(false), useNormalTexture(false), useORM(false), textures()
{
	//consider assigning shader by passing it as a parameter
	shader = Shader::findShader("PBRShader");
	sendToShader();
}

Material::Material(const std::string& name) : baseColor(1, 1, 1), roughness(0.3), metalic(0), name(name),
												hasDiffuse(false), useNormalTexture(false), useORM(false), textures()
{
	//consider assigning shader by passing it as a parameter
	shader = Shader::findShader("PBRShader");
	sendToShader();
}

void Material::sendToShader() const
{
	//glUseProgram(shaderProgram);
	shader->setVec3("baseColor", baseColor);
	shader->setFloat("roughness", roughness);
	shader->setFloat("metalic", metalic);

	shader->setBool("hasDiffuse", hasDiffuse);
	shader->setBool("useNormalTexture", useNormalTexture);
	shader->setBool("useORM", useORM);

	shader->setInt("diffTexture", 0); // Texture unit 0 is for base images.
	shader->setInt("ORMTexture", 1); // Texture unit 1 is for ORM images.
	shader->setInt("normalTexture", 2); // Texture unit 2 is for normal maps.
	//shader->setInt("depthMap", 3); // Texture unit 3 is for depth maps, if needed.

	//This is specific for PBR materials, so it might go to a derived class later
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, *textures[0]);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, *textures[1]);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, *textures[2]);
}