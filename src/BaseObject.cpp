#include "BaseObject.h"

BaseObject::BaseObject() : parentPtr(nullptr)
{
	//Later on add global variable for the scene objects and check if the name is already taken
	name = "New Object";
	setPosition(glm::vec3(0.0f));
	setRotation(glm::vec3(0.0f));
	setScale(glm::vec3(1.0f));
	modelMatrix = glm::mat4(1.0f);
}

BaseObject::BaseObject(const std::string& name) : parentPtr(nullptr)
{
	//Later on add global variable for the scene objects and check if the name is already taken
	this->name = name;
	setPosition(glm::vec3(0.0f));
	setRotation(glm::vec3(0.0f));
	setScale(glm::vec3(1.0f));
	modelMatrix = glm::mat4(1.0f);
}

//BaseObject::BaseObject(const BaseObject& other)
//{
//	name = other.name;
//	transform = other.transform;
//	modelMatrix = other.modelMatrix;
//	children = other.children;
//	parentPtr = other.parentPtr;
//}
//
//BaseObject& BaseObject::operator=(const BaseObject& other)
//{
//	if (this != &other)
//	{
//		name = other.name;
//		transform = other.transform;
//		modelMatrix = other.modelMatrix;
//		children = other.children;
//		parentPtr = other.parentPtr;
//	}
//	return *this;
//}

BaseObject::~BaseObject()
{

}

void BaseObject::setName(const std::string& name)
{
	this->name = name;
}

void BaseObject::translate(const glm::vec3& position)
{
	transform.position += position;
	updateModelMatrix();

	for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].translate(position);
	}	
}

void BaseObject::rotate(const glm::vec3& rotation)
{
	transform.rotation += rotation;
	updateModelMatrix();

	for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].rotate(rotation);
	}
}

void BaseObject::scale(const glm::vec3& scale)
{
	transform.scale *= scale;
	updateModelMatrix();

	for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].scale(scale);
	}
}

void BaseObject::setPosition(const glm::vec3& position)
{
	//glm::vec3 translation = position - transform.position;
	//transform.position += translation;
	transform.position = position;
	updateModelMatrix();

	for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].setPosition(position);
	}
}

void BaseObject::setPosition(float x, float y, float z)
{
	transform.position.x = x;
	transform.position.y = y;
	transform.position.z = z;
	updateModelMatrix();

	for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].setPosition(x, y, z);
	}
}

void BaseObject::setRotation(const glm::vec3& rotation)
{
	glm::vec3 rotationAmount = rotation - transform.rotation;
	transform.rotation += rotationAmount;
	updateModelMatrix();

	for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].rotate(rotationAmount);
	}
}

void BaseObject::setScale(const glm::vec3& scale)
{
	transform.scale = scale;
	updateModelMatrix();

	for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].setScale(scale);
	}
}

const Transform& BaseObject::getTransform() const
{
	return transform;
}

void BaseObject::setParent(BaseObject* parent)
{
	parentPtr = parent;
}

void BaseObject::setTransform(const Transform& otherTransform)
{
	setPosition(otherTransform.position);
	setRotation(otherTransform.rotation);
	setScale(otherTransform.scale);

	for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].setTransform(otherTransform);
	}
}

void BaseObject::updateModelMatrix()
{
	modelMatrix = glm::translate(glm::mat4(1.0f), transform.position);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, transform.scale);
}

BaseObject* BaseObject::addChild(BaseObject& child)
{
	BaseObject* childPtr = children.addObject(child);
	childPtr->parentPtr = this;
	return childPtr;
}

void BaseObject::addChild(BaseObject* child)
{
	child->parentPtr = this;
	children.addObject(child);
}

void BaseObject::removeChild(unsigned index)
{
	children.removeObject(index);
}

void BaseObject::removeChild(BaseObject* object)
{
	children.removeObject(object);
}

//test
void BaseObject::drawDebug() const
{
	if(!debugLinesContainer.isEmpty())
	{
		debugLinesContainer.drawLines(modelMatrix);
	}

	for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].drawDebug();
	}
}

BaseObject& BaseObject::operator[](unsigned index)
{
	return children[index];
}

const BaseObject& BaseObject::operator[](unsigned index) const
{
	return children[index];
}

BaseObject* BaseObject::attachTo(BaseObject& parent)
{
	BaseObject* childPtr = parent.addChild(*this);

	if (parentPtr != nullptr)
	{
		parentPtr->removeChild(this); //this is being deleted
	}
	childPtr->parentPtr = &parent;
	return childPtr;
}

void BaseObject::draw() const
{
	unsigned meshesCount = children.getSize();
	for (size_t i = 0; i < meshesCount; i++)
	{
		children[i].draw();
	}
}