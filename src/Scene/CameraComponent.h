#pragma once
#include "../Resources/Shader.h"
#include "../Renderer/Primitives.h"
#include <GLFW/glfw3.h>
#include <gtc/type_ptr.hpp>

class CameraComponent
{
public:
	glm::vec3 getViewDirection() { return viewDirection; }

	float getFOV() const { return FOV; }
	glm::vec3 getViewDirection() const { return viewDirection; }
	glm::vec3 getRightVector() const { return rightVector; }
	float getNear() const { return near; }
	float getFar() const { return far; }
	float getAspectRatio() const { return aspectRatio; }
	glm::mat4 getViewMat() const { return viewMat; }
	glm::mat4 getPerspectiveMat() const { return perspectiveMat; }
	float getSpeed() const { return cameraSpeed; }

	void setFOV(float fov);
	void setViewDirection(const glm::vec3& viewDirection);
	void setSpeed(float speed);
	void setNear(float near);
	void setFar(float far);
	void setAspectRatio(float width, float height);

	void update(const Transform& transform);

	//TEMP
	void sendToShader(const glm::vec3& position);

public:
	CameraComponent();

private:
	glm::vec3 viewDirection;
	glm::vec3 rightVector;

	glm::mat4 viewMat;
	glm::mat4 perspectiveMat;

	float FOV = 90.0f;
	float cameraSpeed = 1.0f;
	float near;
	float far;
	float aspectRatio;
};