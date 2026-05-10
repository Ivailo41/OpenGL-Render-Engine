#include "LightComponent.h"

LightComponent::LightComponent() : type(LightType::POINT), lightColor(glm::vec3(1.0f, 1.0f, 1.0f)), intensity(1.0f), ambientStrength(0.1f)
{

}

LightComponent::LightComponent(const LightType type) : type(type), lightColor(glm::vec3(1.0f, 1.0f, 1.0f)), intensity(1.0f), ambientStrength(0.1f)
{
}

void LightComponent::setIntensity(float intensity)
{
	if (intensity < 0)
	{
		intensity = 0;
	}
	this->intensity = intensity;
}

void LightComponent::setAmbientStrength(float ambientStrength)
{
	if (ambientStrength < 0)
	{
		ambientStrength = 0;
	}
	this->ambientStrength = ambientStrength;
}

void LightComponent::setLightColor(float r, float g, float b)
{
	lightColor = glm::vec3(r, g, b);
}