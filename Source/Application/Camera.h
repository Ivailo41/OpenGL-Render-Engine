#pragma once
#include "BaseObject.h"
#include <GLFW/glfw3.h>
#include "glm/gtc/type_ptr.hpp"

class Camera : public BaseObject
{
public:
	float getFOV() { return FOV; }
	glm::vec3 getViewDirection() { return viewDirection; }

	void setFOV(float fov);

	float getFOV() const { return FOV; }
	glm::vec3 getViewDirection() const { return viewDirection; }
	glm::vec3 getRightVector() const { return rightVector; }

	void setSpeed(float speed);

	void setViewDirection(const glm::vec3& viewDirection);
	void rotateCam(const glm::vec3& rotation);

	void cameraController(GLFWwindow* window, int winX, int winY);

	void draw() const override;
	BaseObject* clone() const override { return new Camera(*this);}

	void updateCamera();

public:
	Camera();
	Camera(const Camera& other);
	Camera& operator=(const Camera& other);
	~Camera();

private:


private:
	glm::vec3 viewDirection;
	glm::vec3 rightVector;

	glm::mat4 viewMat;
	glm::mat4 perspectiveMat;
	float FOV;

	float cameraSpeed = 0.01f;
};

