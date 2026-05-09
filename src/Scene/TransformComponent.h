#pragma once
#include "../glm/gtc/type_ptr.hpp"
#include "../Renderer/Primitives.h"
#include "../Math/Math.h"

class TransformComponent
{
public:

	TransformComponent();

	void translate(const glm::vec3& translation);
	void setPosition(const glm::vec3& position);
	void setPosition(float, float, float);

	void rotate(const glm::vec3& rotation);
	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);

	void scale(const glm::vec3& scale);
	void setTransform(const Transform& transform);
	void setTransform(const glm::mat4& transformMat);

	glm::vec3 getPosition() const { return transform.position; }
	glm::vec3 getRotation() const { return transform.rotation; }
	glm::vec3 getScale() const { return transform.scale; }
	const glm::mat4& getModelMatrix() const { return modelMatrix; }
	const Transform& getTransform() const { return transform; }

	glm::mat4 getTransformMat() const;

private:
	Transform transform;
	glm::mat4 modelMatrix;
	bool moved;

	// helper to rebuild modelMatrix from transform
	void updateModelMatrix();
};