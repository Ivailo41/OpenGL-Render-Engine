#include "PointLight.h"

PointLight::PointLight() : Light("New Point Light")
{
	shadowCubemap.generateCubemap(SHADOW_WIDTH, SHADOW_HEIGHT, CubemapType::SHADOW_MAP);
	type = LightType::POINT;
}

PointLight::PointLight(const std::string name) : Light(name)
{
	shadowCubemap.generateCubemap(SHADOW_WIDTH, SHADOW_HEIGHT, CubemapType::SHADOW_MAP);
	type = LightType::POINT;
}

PointLight::~PointLight()
{

}

const PointLight::PointLightData* PointLight::getData() const
{
	return &data;
}

void PointLight::sendToShader(const Shader& shaderProgram, unsigned lightIndex) const
{
	unsigned posLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + std::to_string(lightIndex) + "].position").c_str());
	glUniform3f(posLoc, transformComponentPtr->getPosition().x, transformComponentPtr->getPosition().y, transformComponentPtr->getPosition().z);

	unsigned intLoc = glGetUniformLocation(shaderProgram, ("pointLights[" + std::to_string(lightIndex) + "].intensity").c_str());
	glUniform1f(intLoc, getIntensity());
}

void PointLight::sendShadowDataToShader(const Shader& shaderProgram, unsigned lightIndex) const
{
	GLuint shadowMatricesLoc = glGetUniformLocation(shaderProgram, "shadowMatrices");
	glUniformMatrix4fv(shadowMatricesLoc, 6, GL_FALSE, glm::value_ptr(shadowMatrices[0]));

	shaderProgram.setVec3("lightPosition", transformComponentPtr->getPosition());
	shaderProgram.setFloat("far_plane", shadowFar);
}

void PointLight::draw(Shader* overrideShader, GLenum drawMode) const
{
	Light::draw(overrideShader);
}

void PointLight::update(float deltaTime)
{
	float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, shadowNear, shadowFar);

	shadowMatrices[0] = shadowProj * glm::lookAt(transformComponentPtr->getPosition(), transformComponentPtr->getPosition() + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	shadowMatrices[1] = shadowProj * glm::lookAt(transformComponentPtr->getPosition(), transformComponentPtr->getPosition() + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)); //the wrong one
	shadowMatrices[2] = shadowProj * glm::lookAt(transformComponentPtr->getPosition(), transformComponentPtr->getPosition() + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //top
	shadowMatrices[3] = shadowProj * glm::lookAt(transformComponentPtr->getPosition(), transformComponentPtr->getPosition() + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));//bottom
	shadowMatrices[4] = shadowProj * glm::lookAt(transformComponentPtr->getPosition(), transformComponentPtr->getPosition() + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));//not
	shadowMatrices[5] = shadowProj * glm::lookAt(transformComponentPtr->getPosition(), transformComponentPtr->getPosition() + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));//not
}
