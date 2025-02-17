#pragma once
#include "BaseObject.h"
#include <GLFW/glfw3.h>
#include <gtc/type_ptr.hpp>

class Camera : public BaseObject
{
public:
	glm::vec3 getViewDirection() { return viewDirection; }

	//might make the camera properties public, there is no need of these getters
	float getFOV() const { return FOV; }
	glm::vec3 getViewDirection() const { return viewDirection; }
	glm::vec3 getRightVector() const { return rightVector; }
	float getNear() const { return near; }
	float getFar() const { return far; }
	float getAspectRatio() const { return aspectRatio; }
	glm::mat4 getViewMat() const { return viewMat; }
	glm::mat4 getPerspectiveMat() const { return perspectiveMat; }

	void setFOV(float fov);
	void setSpeed(float speed);
	void setViewDirection(const glm::vec3& viewDirection);
	void setNear(float near);
	void setFar(float far);
	void setAspectRatio(float width, float height);

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
	float near, far;
	float aspectRatio;
};

