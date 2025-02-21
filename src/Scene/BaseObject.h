#pragma once

//#define GLEW_STATIC
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/matrix_decompose.hpp"

#include <string>
#include "../Renderer/Primitives.h"
#include <vector>

#include "../Math/Math.h"

#include "../Renderer/LinesContainer.h"

class BaseObject
{
public:
	void setName(const std::string& name);
	std::string getName() const { return name; }

	//later move these into the transform struct

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
	const Transform& getTransform() const;

	//returns a model matrix from the object's transform, consider using it instead the modelMatrix variable in Mesh class
	//TEMP
	glm::mat4 getTransformMat() const;


	const glm::mat4 getGlobalModelMat();
	const glm::mat4 globalToLocalMat(const glm::mat4& matrix) const;

	virtual void draw() const;
	virtual BaseObject* clone() const { return new BaseObject(*this); }

	void setParent(BaseObject* parent);
	BaseObject* getParent() const { return parentPtr; }

	unsigned getChildrenCount() const { return children.size(); }
	void attachTo(BaseObject* parent);

	bool isChildOf(const BaseObject* object) const;

public:
	BaseObject();
	BaseObject(const std::string& name);
	virtual ~BaseObject();

public:
	void addChild(BaseObject& child);
	void addChild(BaseObject* child);

	//doesnt delete the object!!!!
	void removeChild(unsigned index);
	void removeChild(BaseObject* object);

	//test
	void drawDebug() const;

	LinesContainer debugLinesContainer;
	std::vector<BaseObject*> children;

protected:
	std::string name;
	Transform transform;

	BaseObject* parentPtr;

private:
	void updateModelMat();
};

