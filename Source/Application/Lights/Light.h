#pragma once
#include "../BaseObject.h"
#include "../Mesh.h"
#include "../Shader.h"

class Light : public BaseObject
{
public:
	Light();
	Light(const std::string name);
	Light(const Light& other);
	Light& operator=(const Light& other);
	virtual ~Light() override;

	void setIntensity(float intensity);
	void setAmbientStrength(float ambientStrength);
	void setLightColor(char r, char g, char b);

	float getIntensity() const { return intensity; }
	float getAmbientStrength() const { return ambientStrength; }
	glm::vec3 getLightColor() const { return lightColor; }

	float* ambientRef() { return &ambientStrength; }

	virtual void draw() const override;
	virtual BaseObject* clone() const override { return new Light(*this);}

	virtual void sendToShader(unsigned shaderProgram, unsigned lightIndex) const;

protected:
	float intensity;
	float ambientStrength;
	glm::vec3 lightColor;
};

