#include "Material.h"
#include <iostream>
#include "Shader.h"

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
	shaderProgram = *Shader::findShader("PBRShader");
	sendToShader();
}

Material::Material(const std::string& name) : baseColor(1, 1, 1), roughness(0.3), metalic(0), name(name),
												hasDiffuse(false), useNormalTexture(false), useORM(false), textures()
{
	//consider assigning shader by passing it as a parameter
	shaderProgram = *Shader::findShader("PBRShader");
	sendToShader();
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