#pragma once
#include "../BaseObject.h"
#include "../Mesh.h"
#include "../../Renderer/Shader.h"

class Light : public BaseObject
{
public:
	Light();
	Light(const std::string name);
	//Light(const Light& other);
	//Light& operator=(const Light& other);
	virtual ~Light() override;

	void setIntensity(float intensity);
	void setAmbientStrength(float ambientStrength);
	void setLightColor(char r, char g, char b);

	float getIntensity() const { return intensity; }
	float getAmbientStrength() const { return ambientStrength; }
	glm::vec3 getLightColor() const { return lightColor; }

	float* ambientRef() { return &ambientStrength; }

	virtual void draw(Shader* overrideShader = nullptr) const override;
	virtual void update(float deltaTime) override;
	virtual void sendToShader(const Shader& shaderProgram, unsigned lightIndex) const = 0;
	virtual void sendShadowDataToShader(const Shader& shaderProgram, unsigned lightIndex) const = 0;

public:
	static uint16_t SHADOW_WIDTH;
	static uint16_t SHADOW_HEIGHT;

protected:
	float intensity;
	float ambientStrength;
	glm::vec3 lightColor;

};

