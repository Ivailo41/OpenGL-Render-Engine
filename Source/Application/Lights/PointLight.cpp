#include "PointLight.h"

PointLight::PointLight() : Light("New Point Light")
{
	setConstant(1.0f);
	setLinear(0.09f);
	setQuadratic(0.032f);
}

PointLight::PointLight(const std::string name) : Light(name)
{
	setConstant(1.0f);
	setLinear(0.09f);
	setQuadratic(0.032f);
}

PointLight::PointLight(const PointLight& other) : Light(other)
{
	Light::operator=(other);
	data = other.data;
}

PointLight& PointLight::operator=(const PointLight& other)
{
	if (this != &other)
	{
		Light::operator=(other);
		data = other.data;
	}
	return *this;
}

PointLight::~PointLight()
{

}

void PointLight::setConstant(float constant)
{
	if (constant < 0)
	{
		constant = 0;
	}
	this->data.constant = constant;
}

void PointLight::setLinear(float linear)
{
	if (linear < 0)
	{
		linear = 0;
	}
	this->data.linear = linear;
}

void PointLight::setQuadratic(float quadratic)
{
	if (quadratic < 0)
	{
		quadratic = 0;
	}
	this->data.quadratic = quadratic;
}

const PointLight::pointLightData* PointLight::getData() const
{
	return &data;
}

void PointLight::sendToShader(unsigned shaderProgram, unsigned lightIndex) const
{
	unsigned posLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + std::to_string(lightIndex) + "].position").c_str());
	glUniform3f(posLoc, getPosition().x, getPosition().y, getPosition().z);

	unsigned constLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + std::to_string(lightIndex) + "].constant").c_str());
	glUniform1f(constLoc, getConstant());

	unsigned linLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + std::to_string(lightIndex) + "].linear").c_str());
	glUniform1f(linLoc, getLinear());

	unsigned quadLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + std::to_string(lightIndex) + "].quadratic").c_str());
	glUniform1f(quadLoc, getQuadratic());
}

void PointLight::draw() const
{
	Light::draw();
	//sendToShader(Shader::activeShader->getShaderProgram(), 0);
}