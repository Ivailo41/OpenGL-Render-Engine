#include "Light.h"

Light::Light() : BaseObject("New Light")
{
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	intensity = 1.0f;
	ambientStrength = 0.1f;
}

Light::Light(const std::string name) : BaseObject(name)
{
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	intensity = 1.0f;
	ambientStrength = 0.1f;
}

Light::Light(const Light& other) : BaseObject(other)
{
	lightColor = other.lightColor;
	intensity = other.intensity;
	ambientStrength = other.ambientStrength;
}

Light& Light::operator=(const Light& other)
{
	if(this != &other)
	{
		BaseObject::operator=(other);
		lightColor = other.lightColor;
		intensity = other.intensity;
		ambientStrength = other.ambientStrength;
	}
	return *this;
}

Light::~Light()
{

}

void Light::setIntensity(float intensity)
{
	if(intensity < 0)
	{
		intensity = 0;
	}
	this->intensity = intensity;
}

void Light::setAmbientStrength(float ambientStrength)
{
	if (ambientStrength < 0)
	{
		ambientStrength = 0;
	}
	this->ambientStrength = ambientStrength;
}

void Light::setLightColor(char r, char g, char b)
{
	lightColor = glm::vec3(r, g, b);
}

void Light::draw() const
{

}

void Light::sendToShader(unsigned shaderProgram, unsigned lightIndex)const
{

}