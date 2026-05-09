#include "TransformComponent.h"

TransformComponent::TransformComponent() : transform(), modelMatrix(1.0f), moved(false)
{
	updateModelMatrix();
}

void TransformComponent::updateModelMatrix()
{
	// Apply rotations in Z, Y, X order (match comments/expectation)
	glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotation = rotZ * rotY * rotX;

	modelMatrix = glm::translate(glm::mat4(1.0f), transform.position)
		* rotation
		* glm::scale(glm::mat4(1.0f), transform.scale);
}

void TransformComponent::translate(const glm::vec3& position)
{
	transform.position += position;
	moved = true;
	updateModelMatrix();
}

void TransformComponent::rotate(const glm::vec3& rotation)
{
	// rotation is the delta in degrees
	transform.rotation += rotation;
	moved = true;
	updateModelMatrix();
}

void TransformComponent::scale(const glm::vec3& scale)
{
	transform.scale *= scale;
	moved = true;
	updateModelMatrix();
}

void TransformComponent::setPosition(const glm::vec3& position)
{
	transform.position = position;
	moved = true;
	updateModelMatrix();
}

void TransformComponent::setPosition(float x, float y, float z)
{
	transform.position = glm::vec3(x, y, z);
	moved = true;
	updateModelMatrix();
}

void TransformComponent::setRotation(const glm::vec3& rotation)
{
	// rotation provided in degrees
	transform.rotation = rotation;
	moved = true;
	updateModelMatrix();
}

void TransformComponent::setScale(const glm::vec3& scale)
{
	transform.scale = scale;
	moved = true;
	updateModelMatrix();
}

void TransformComponent::setTransform(const Transform& otherTransform)
{
	transform = otherTransform;
	moved = true;
	updateModelMatrix();
}

void TransformComponent::setTransform(const glm::mat4& transformMat)
{
	// Decompose into position, (rad)rotation, scale
	glm::vec3 rotRad;
	Math::DecomposeMatrix(transformMat, transform.position, rotRad, transform.scale);

	// store rotation in degrees (consistent with other setters)
	transform.rotation.x = glm::degrees(rotRad.x);
	transform.rotation.y = glm::degrees(rotRad.y);
	transform.rotation.z = glm::degrees(rotRad.z);

	moved = true;
	updateModelMatrix();
}

glm::mat4 TransformComponent::getTransformMat() const
{
	// Build from transform values (same as updateModelMatrix) and return
	glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotation = rotZ * rotY * rotX;

	return glm::translate(glm::mat4(1.0f), transform.position)
		* rotation
		* glm::scale(glm::mat4(1.0f), transform.scale);
}