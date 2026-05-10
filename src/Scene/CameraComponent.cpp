#include "CameraComponent.h"

void CameraComponent::setFOV(float fov)
{
	if (fov < 0)
		fov = 0;
	else if (fov > 180)
		fov = 180;
	FOV = fov;

	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far); //FIX ASPECT RATIO
}

CameraComponent::CameraComponent() : near(0.1f), far(100.0f), aspectRatio(4.0f / 3.0f), viewDirection(0.0f, 0.0f, -1.0f), viewMat(1.0f), perspectiveMat(1.0f),
	rightVector(1.0f, 0.0f, 0.0f)
{
	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far);
}

void CameraComponent::update(const Transform& transform)
{
	// Recompute view direction from yaw (horizontal) and pitch (vertical).
	// Convert stored degrees -> radians for trig.
	float yawRad = glm::radians(transform.rotation.y);
	float pitchRad = glm::radians(transform.rotation.x);

	// standard spherical to Cartesian conversion (adjust signs/order to taste)
	glm::vec3 dir;
	dir.x = cos(pitchRad) * sin(yawRad);
	dir.y = sin(pitchRad);
	dir.z = cos(pitchRad) * cos(yawRad);

	setViewDirection(glm::normalize(dir));

	// right and up vectors
	const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	rightVector = glm::normalize(glm::cross(viewDirection, worldUp));

	viewMat = glm::lookAt(transform.position, transform.position + viewDirection, glm::vec3(0.0f, 1.0f, 0.0f));
}

void CameraComponent::sendToShader(const glm::vec3& position)
{
	unsigned int directionLoc = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "viewDirection");
	glUniform3f(directionLoc, getViewDirection().x, getViewDirection().y, getViewDirection().z);

	unsigned int viewMatrixLoc = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "viewMat");
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMat));

	unsigned int perspectiveMatLoc = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "perspectiveMat");
	glUniformMatrix4fv(perspectiveMatLoc, 1, GL_FALSE, glm::value_ptr(perspectiveMat));

	unsigned int camPos = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "camPos");
	glUniform3f(camPos, position.x, position.y, position.z);
}


void CameraComponent::setViewDirection(const glm::vec3& viewDirection)
{
	this->viewDirection = viewDirection;
}

void CameraComponent::setSpeed(float speed)
{
	cameraSpeed = speed;
}

void CameraComponent::setNear(float near)
{
	this->near = near;
	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far);
}

void CameraComponent::setFar(float far)
{
	this->far = far;
	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far);
}

void CameraComponent::setAspectRatio(float width, float heigth)
{
	aspectRatio = width / heigth;
	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far);
}