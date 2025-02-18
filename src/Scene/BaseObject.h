#pragma once

//#define GLEW_STATIC
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <string>
#include "../Renderer/Primitives.h"
#include <vector>

#include "../Renderer/LinesContainer.h"

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

	virtual void draw() const;
	virtual BaseObject* clone() const { return new BaseObject(*this); }

	unsigned getChildrenCount() const { return children.size(); }
	void attachTo(BaseObject* parent);

public:
	BaseObject();
	BaseObject(const std::string& name);
	virtual ~BaseObject();

public:
	void addChild(BaseObject& child);
	void addChild(BaseObject* child);

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
	void updateModelMatrix();
};

