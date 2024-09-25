#include "PointLight.h"

PointLight::PointLight() : Light("New Point Light")
{

}

PointLight::PointLight(const std::string name) : Light(name)
{

}

PointLight::~PointLight()
{

}

const PointLight::pointLightData* PointLight::getData() const
{
	return &data;
}

void PointLight::sendToShader(unsigned shaderProgram, unsigned lightIndex) const
{
	unsigned posLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + std::to_string(lightIndex) + "].position").c_str());
	glUniform3f(posLoc, getPosition().x, getPosition().y, getPosition().z);

	unsigned intLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + std::to_string(lightIndex) + "].intensity").c_str());
	glUniform1f(intLoc, getIntensity());
}

void PointLight::draw() const
{
	Light::draw();
	//sendToShader(Shader::activeShader->getShaderProgram(), 0);
}