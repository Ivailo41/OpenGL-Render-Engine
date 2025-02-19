#include "BaseObject.h"

BaseObject::BaseObject() : parentPtr(nullptr)
{
	name = "New Object";
	setPosition(glm::vec3(0.0f));
	setRotation(glm::vec3(0.0f));
	setScale(glm::vec3(1.0f));
	//modelMatrix = glm::mat4(1.0f);
}

BaseObject::BaseObject(const std::string& name) : parentPtr(nullptr)
{
	this->name = name;
	setPosition(glm::vec3(0.0f));
	setRotation(glm::vec3(0.0f));
	setScale(glm::vec3(1.0f));
	//modelMatrix = glm::mat4(1.0f);
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

	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].translate(position);
	}*/	
}

void BaseObject::rotate(const glm::vec3& rotation)
{
	transform.rotation += rotation;
	updateModelMatrix();

	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].rotate(rotation);
	}*/
}

void BaseObject::scale(const glm::vec3& scale)
{
	transform.scale *= scale;
	updateModelMatrix();

	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].scale(scale);
	}*/
}

void BaseObject::setPosition(const glm::vec3& position)
{
	//glm::vec3 translation = position - transform.position;
	//transform.position += translation;
	transform.position = position;
	updateModelMatrix();

	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].setPosition(position);
	}*/
}

void BaseObject::setPosition(float x, float y, float z)
{
	transform.position.x = x;
	transform.position.y = y;
	transform.position.z = z;
	updateModelMatrix();

	//TO DO: dont update the children location but multiply their matrices with parent's
	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].setPosition(x, y, z);
	}*/
}

void BaseObject::setRotation(const glm::vec3& rotation)
{
	glm::vec3 rotationAmount = rotation - transform.rotation;
	transform.rotation += rotationAmount;
	updateModelMatrix();

	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].rotate(rotationAmount);
	}*/
}

void BaseObject::setScale(const glm::vec3& scale)
{
	transform.scale = scale;
	updateModelMatrix();

	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].setScale(scale);
	}*/
}

const Transform& BaseObject::getTransform() const
{
	return transform;
}

const glm::mat4 BaseObject::getModelMatrix()
{
	//add flag if the matrix should be recalculated so we dont calculate it each time
	updateModelMatrix();
	return transform.modelMatrix;
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

	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].setTransform(otherTransform);
	}*/
}

void BaseObject::updateModelMatrix()
{
	//update the matrix with the current transform
	transform.modelMatrix = glm::translate(glm::mat4(1.0f), transform.position);
	transform.modelMatrix = glm::rotate(transform.modelMatrix, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform.modelMatrix = glm::rotate(transform.modelMatrix, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform.modelMatrix = glm::rotate(transform.modelMatrix, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transform.modelMatrix = glm::scale(transform.modelMatrix, transform.scale);

	//multiply by the parents matrix
	transform.modelMatrix = parentPtr ? parentPtr->getModelMatrix() * transform.modelMatrix : transform.modelMatrix;
}

void BaseObject::addChild(BaseObject& child)
{
	children.push_back(&child);
	child.parentPtr = this;
}

void BaseObject::addChild(BaseObject* child)
{
	child->parentPtr = this;
	children.push_back(child);
}

void BaseObject::removeChild(unsigned index)
{
	children.erase(children.begin() + index);
}

void BaseObject::removeChild(BaseObject* object)
{
	unsigned childrenCount = children.size();
	for (size_t i = 0; i < childrenCount; i++)
	{
		if(object == children[i])
		{
			children.erase(children.begin() + i);
			break;
		}
	}
}

//test
void BaseObject::drawDebug() const
{
	if(!debugLinesContainer.isEmpty())
	{
		debugLinesContainer.drawLines(transform.modelMatrix);
	}

	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->drawDebug();
	}
}

void BaseObject::attachTo(BaseObject* parent)
{
	//make a check if the dragged object is not a parrent of the target
	if(parentPtr == parent)
	{
		return;
	}

	if (parentPtr)
	{
		parentPtr->removeChild(this);
	}
	else
	{
		//if it has no parent then it is inside the mainscene, either make the root of the main scene an baseobject or think how to delete the object from here
	}
	parent->addChild(this);
	parentPtr = parent;
}

void BaseObject::draw() const
{
	unsigned meshesCount = children.size();
	for (size_t i = 0; i < meshesCount; i++)
	{
		children[i]->draw();
	}
}