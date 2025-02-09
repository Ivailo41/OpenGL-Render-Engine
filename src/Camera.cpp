#include "Camera.h"
#include "Shader.h"

void Camera::setFOV(float fov)
{
	if(fov < 0)
		fov = 0;
	else if(fov > 180)
		fov = 180;
	FOV = fov;

	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far); //FIX ASPECT RATIO
}

Camera::Camera() : BaseObject("New Camera"), near(0.1f), far(100.0f), aspectRatio(4.0f/3.0f)
{
	setFOV(45.0f);
	viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	rightVector = glm::vec3(1.0f, 0.0f, 0.0f);
	perspectiveMat = glm::perspective(glm::radians(FOV), aspectRatio, near, far);
}

Camera::Camera(const Camera& other) : BaseObject(other), near(other.near), far(other.far), aspectRatio(other.aspectRatio)
{
	viewDirection = other.viewDirection;
	viewMat = other.viewMat;
	rightVector = other.rightVector;
	perspectiveMat = other.perspectiveMat;
	FOV = other.FOV;
}

Camera& Camera::operator=(const Camera& other)
{
	if (this != &other)
	{
		BaseObject::operator=(other);
		viewDirection = other.viewDirection;
		viewMat = other.viewMat;
		rightVector = other.rightVector;
		perspectiveMat = other.perspectiveMat;
		FOV = other.FOV;

		near = other.near;
		far = other.far;
		aspectRatio = other.aspectRatio;
	}
	return *this;
}

Camera::~Camera()
{
}

void Camera::rotateCam(const glm::vec3& rotation)
{
	BaseObject::rotate(rotation);

	float horizontalAngle = transform.rotation.y;
	float verticalAngle = transform.rotation.x;

	setViewDirection(glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)));

	rightVector += glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle));

	//updateCamera();
}

void Camera::updateCamera()
{
	viewMat = glm::lookAt(transform.position, transform.position + viewDirection, glm::vec3(0.0f, 1.0f, 0.0f));

	unsigned int directionLoc = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "viewDirection");
	glUniform3f(directionLoc, getViewDirection().x, getViewDirection().y, getViewDirection().z);

	unsigned int viewMatrixLoc = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "viewMat");
	glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMat));

	unsigned int perspectiveMatLoc = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "perspectiveMat");
	glUniformMatrix4fv(perspectiveMatLoc, 1, GL_FALSE, glm::value_ptr(perspectiveMat));

	unsigned int camPos = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "camPos");
	glUniform3f(camPos, getPosition().x, getPosition().y, getPosition().z);
}

void Camera::draw() const
{
}

void Camera::setViewDirection(const glm::vec3& viewDirection)
{
	this->viewDirection = viewDirection;
	//updateCamera();
}

void Camera::cameraController(GLFWwindow* window, int winX, int winY)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		translate(getViewDirection() * cameraSpeed);
		//position += direction * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		translate(-getViewDirection() * cameraSpeed);
		//position -= direction * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		transform.position -= glm::cross(viewDirection, glm::vec3(0, 1, 0)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		transform.position += glm::cross(viewDirection, glm::vec3(0, 1, 0)) * cameraSpeed;
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	glfwSetCursorPos(window, winX / 2, winY / 2);

	float horizontalAngle = 0.001f * float(winX / 2 - xpos);
	float verticalAngle = 0.001f * float(winY / 2 - ypos);

	rotateCam(glm::vec3(verticalAngle, horizontalAngle, 0));
	//updateCamera();
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