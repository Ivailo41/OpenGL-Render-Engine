#include "Camera.h"
#include "../Resources/Shader.h"

void Camera::setFOV(float fov)
{
	if(fov < 0)
		fov = 0;
	else if(fov > 180)
		fov = 180;
	FOV = fov;

	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far); //FIX ASPECT RATIO
}

Camera::Camera() : SceneNode("New Camera"), near(0.1f), far(100.0f), aspectRatio(4.0f/3.0f), viewDirection(0.0f, 0.0f, -1.0f),
	rightVector(1.0f, 0.0f, 0.0f)
{
	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far);
	viewMat = glm::lookAt(transformComponentPtr->getPosition(), transformComponentPtr->getPosition() + viewDirection, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::rotateCam(const glm::vec3& rotation)
{
	// rotation.x = deltaPitch (degrees), rotation.y = deltaYaw (degrees)
	glm::vec3 currentRot = transformComponentPtr->getRotation();

	// clamp pitch (x) to avoid gimbal flip. Use degrees here (TransformComponent stores degrees).
	const float maxPitch = 89.0f;
	float desiredPitch = currentRot.x + rotation.x;
	if (desiredPitch > maxPitch) desiredPitch = maxPitch;
	if (desiredPitch < -maxPitch) desiredPitch = -maxPitch;

	// compute deltas we will actually apply (apply rotation exactly once)
	float appliedDeltaPitch = desiredPitch - currentRot.x;
	float appliedDeltaYaw = rotation.y;

	// Apply rotation once (we assume TransformComponent::rotate accepts degrees deltas)
	transformComponentPtr->rotate(glm::vec3(appliedDeltaPitch, appliedDeltaYaw, 0.0f));

	// Recompute view direction from yaw (horizontal) and pitch (vertical).
	// Convert stored degrees -> radians for trig.
	float yawRad = glm::radians(transformComponentPtr->getRotation().y);
	float pitchRad = glm::radians(transformComponentPtr->getRotation().x);

	// standard spherical to Cartesian conversion (adjust signs/order to taste)
	glm::vec3 dir;
	dir.x = cos(pitchRad) * sin(yawRad);
	dir.y = sin(pitchRad);
	dir.z = cos(pitchRad) * cos(yawRad);

	setViewDirection(glm::normalize(dir));

	// right and up vectors
	const glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	rightVector = glm::normalize(glm::cross(viewDirection, worldUp));
	// optional: recompute orthonormal up if needed
	// glm::vec3 recalculatedUp = glm::normalize(glm::cross(rightVector, viewDirection));
}

void Camera::updateCamera()
{
	unsigned int directionLoc = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "viewDirection");
	glUniform3f(directionLoc, getViewDirection().x, getViewDirection().y, getViewDirection().z);

	unsigned int viewMatrixLoc = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "viewMat");
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMat));

	unsigned int perspectiveMatLoc = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "perspectiveMat");
	glUniformMatrix4fv(perspectiveMatLoc, 1, GL_FALSE, glm::value_ptr(perspectiveMat));

	unsigned int camPos = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "camPos");
	glUniform3f(camPos, transformComponentPtr->getPosition().x, transformComponentPtr->getPosition().y, transformComponentPtr->getPosition().z);
}

void Camera::draw(Shader* overrideShader, GLenum drawMode) const
{
}

void Camera::update(float deltaTime)
{

}

void Camera::setViewDirection(const glm::vec3& viewDirection)
{
	this->viewDirection = viewDirection;
}

void Camera::cameraController(GLFWwindow* window, int winX, int winY, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		transformComponentPtr->translate(getViewDirection() * cameraSpeed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		transformComponentPtr->translate(-getViewDirection() * cameraSpeed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		transformComponentPtr->translate(-rightVector * cameraSpeed * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		transformComponentPtr->translate(rightVector * cameraSpeed * deltaTime);
	}

	// Read cursor and re-center it
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, winX / 2, winY / 2);

	// Sensitivity: degrees per pixel.
	const float sensitivity = 0.1f; // 0.1 degree per pixel

	float deltaYawDeg = sensitivity * float((winX / 2) - xpos);   // horizontal mouse movement
	float deltaPitchDeg = sensitivity * float((winY / 2) - ypos); // vertical mouse movement

	// Pass deltas in degrees
	rotateCam(glm::vec3(deltaPitchDeg, deltaYawDeg, 0.0f));

	// Update view matrix after rotation/movement
	viewMat = glm::lookAt(transformComponentPtr->getPosition(), transformComponentPtr->getPosition() + viewDirection, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::setSpeed(float speed)
{
	cameraSpeed = speed;
}

void Camera::setNear(float near)
{
	this->near = near;
	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far);
}

void Camera::setFar(float far)
{
	this->far = far;
	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far);
}

void Camera::setAspectRatio(float width, float heigth)
{
	aspectRatio = width / heigth;
	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far);
}