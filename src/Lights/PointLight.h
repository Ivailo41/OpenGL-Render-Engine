#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	struct pointLightData
	{
		glm::vec3 position;
		/*float constant;
		float linear;
		float quadratic;*/
	};

public:
	/*void setConstant(float constant);
	void setLinear(float linear);
	void setQuadratic(float quadratic);*/

	/*float getConstant() const { return data.constant; }
	float getLinear() const { return data.linear; }
	float getQuadratic() const { return data.quadratic; }*/

	const pointLightData* getData() const;
	void sendToShader(unsigned shaderProgram, unsigned lightIndex) const override;
	virtual BaseObject* clone() const override { return new PointLight(*this); }
	virtual void draw() const override;

	PointLight();
	PointLight(const std::string name);
	//PointLight(const PointLight& other);
	//PointLight& operator=(const PointLight& other);
	~PointLight();

private:
	pointLightData data;
};

