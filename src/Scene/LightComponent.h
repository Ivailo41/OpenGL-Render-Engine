#pragma once
#include "../Resources/Shader.h"

enum class LightType
{
	POINT,
	DIRECTIONAL,
	SPOTLIGHT,
	UNDEFINED = -1
};

struct PointLightData
{
	
};

class LightComponent {

public:
	LightComponent();
	LightComponent(const LightType type);

	void setIntensity(float intensity);
	void setAmbientStrength(float ambientStrength);
	void setLightColor(float r, float g, float b);
	void setLightType(LightType type) { this->type = type; }
	void setShadowNear(float shadowNear) { this->shadowNear = shadowNear; }
	void setShadowFar(float shadowFar) { this->shadowFar = shadowFar; }

	float getIntensity() const { return intensity; }
	float getAmbientStrength() const { return ambientStrength; }
	glm::vec3 getLightColor() const { return lightColor; }
	LightType getLightType() const { return type; }
	float getShadowNear() const { return shadowNear; }
	float getShadowFar() const { return shadowFar; }

	float* ambientRef() { return &ambientStrength; }

protected:
	float intensity;
	float ambientStrength;
	glm::vec3 lightColor;

	float shadowNear = 0.01f;
	float shadowFar = 25.0f;

	LightType type;

	PointLightData pointLightData;
};