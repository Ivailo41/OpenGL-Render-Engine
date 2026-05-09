#pragma once
#include "Light.h"
#include "../../Renderer/Cubemap.h"

class PointLight : public Light
{
public:
	struct PointLightData
	{
		glm::vec3 position;
		/*float constant;
		float linear;
		float quadratic;*/

		PointLightData() : position(0)
		{
			//nothing to do here
		}
	};

public:
	/*void setConstant(float constant);
	void setLinear(float linear);
	void setQuadratic(float quadratic);*/

	/*float getConstant() const { return data.constant; }
	float getLinear() const { return data.linear; }
	float getQuadratic() const { return data.quadratic; }*/

	const Cubemap& getShadowCubemap() const { return shadowCubemap; }
	float getShadowNear() const { return shadowNear; }
	float getShadowFar() const { return shadowFar; }

	const PointLightData* getData() const;
	void sendToShader(const Shader& shaderProgram, unsigned lightIndex) const override;
	void sendShadowDataToShader(const Shader& shaderProgram, unsigned lightIndex) const override;
	SceneNode* clone() const override { return new PointLight(*this); }
	void draw(Shader* overrideShader = nullptr, GLenum drawMode = GL_TRIANGLES) const override;
	void update(float deltaTime) override;

	PointLight();
	PointLight(const std::string name);
	//PointLight(const PointLight& other);
	//PointLight& operator=(const PointLight& other);
	~PointLight();

private:
	PointLightData data;

	Cubemap shadowCubemap;
	glm::mat4 shadowMatrices[6];
	float shadowNear = 0.01f;
	float shadowFar = 25.0f;
};

