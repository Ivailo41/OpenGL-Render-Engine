#pragma once

//#define GLEW_STATIC
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <string>
#include "Primitives.h"
#include <vector>

#include "ObjectArray.h"
#include "Debug/LinesContainer.h"

class BaseObject
{
public:
	void setName(const std::string& name);
	std::string getName() const { return name; }

	//later move these into the transform struct
	void translate(const glm::vec3& translation);
	void rotate(const glm::vec3& rotation);
	void scale(const glm::vec3& scale);

	void setPosition(const glm::vec3& position);
	void setPosition(float, float, float);

	void setRotation(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);

	glm::vec3 getPosition() const { return transform.position; }
	glm::vec3 getRotation() const { return transform.rotation; }
	glm::vec3 getScale() const { return transform.scale; }

	void setTransform(const Transform& transform);
	const Transform& getTransform() const;

	const glm::mat4 getModelMatrix();

	void setParent(BaseObject* parent);
	BaseObject* getParent() const { return parentPtr; }

	virtual void draw() const = 0;
	virtual BaseObject* clone() const = 0;

	//Functions for the children holder
	BaseObject& operator[](unsigned index);
	const BaseObject& operator[](unsigned index) const;
	
	unsigned getChildrenCount() const { return children.getSize(); }
	
	BaseObject* attachTo(BaseObject& parent);

public:
	BaseObject();
	BaseObject(const std::string& name);
	//BaseObject(const BaseObject& other);
	//BaseObject& operator=(const BaseObject& other);
	virtual ~BaseObject();

public:
	BaseObject* addChild(BaseObject& child);
	void addChild(BaseObject* child);

	void removeChild(unsigned index);
	void removeChild(BaseObject* object);

	//test
	void drawDebug() const;

	LinesContainer debugLinesContainer;

protected:
	std::string name;
	Transform transform;
	//glm::mat4 modelMatrix;

	//Consider using vecotr here
	ObjectArray<BaseObject> children;
	BaseObject* parentPtr;

private:
	void updateModelMatrix();
};

