#pragma once
#include "SceneNode.h"
#include <GLFW/glfw3.h>
#include <gtc/type_ptr.hpp>

class Camera : public SceneNode
{
public:
	struct CursorData
	{
		int winX, winY, winOffsetX, winOffsetY, mouseX, mouseY;
		CursorData(int winX, int winY, int winOffsetX, int winOffsetY, int mouseX, int mouseY)
			: winX(winX), winY(winY), winOffsetX(winOffsetX), winOffsetY(winOffsetY), mouseX(mouseX), mouseY(mouseY)
		{}
	};

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

	float getSpeed() const { return cameraSpeed; }

	void setFOV(float fov);
	void setSpeed(float speed);
	void setViewDirection(const glm::vec3& viewDirection);
	void setNear(float near);
	void setFar(float far);
	void setAspectRatio(float width, float height);

	void rotateCam(const glm::vec3& rotation);

	void cameraController(GLFWwindow* window, int winX, int winY, float deltaTime);

	void draw(Shader* overrideShader = nullptr, GLenum drawMode = GL_TRIANGLES) const override;
	void update(float deltaTime);
	SceneNode* clone() const override { return new Camera(*this);}

	void updateCamera();

public:
	Camera();
	virtual ~Camera() = default;

private:


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

