#include "Light.h"

uint16_t Light::SHADOW_WIDTH = 1024;
uint16_t Light::SHADOW_HEIGHT = 1024;

Light::Light() : SceneNode("New Light")
{
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	intensity = 1.0f;
	ambientStrength = 0.1f;
}

Light::Light(const std::string name) : SceneNode(name)
{
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	intensity = 1.0f;
	ambientStrength = 0.1f;
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

void Light::draw(Shader* overrideShader, GLenum drawMode) const
{

}

void Light::update(float deltaTime)
{

}
