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
	updateModelMat();

	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].translate(position);
	}*/	
}

void BaseObject::rotate(const glm::vec3& rotation)
{
	transform.rotation += rotation;
	updateModelMat();

	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].rotate(rotation);
	}*/
}

void BaseObject::scale(const glm::vec3& scale)
{
	transform.scale *= scale;
	updateModelMat();

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
	updateModelMat();

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
	updateModelMat();

	//TO DO: dont update the children location but multiply their matrices with parent's
	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].setPosition(x, y, z);
	}*/
}

void BaseObject::setRotation(const glm::vec3& rotation)
{
	//glm::quat rotationAmount = rotation - transform.rotation;
	//transform.rotation += rotationAmount;
	transform.rotation = rotation;
	updateModelMat();

	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].rotate(rotationAmount);
	}*/
}

void BaseObject::setScale(const glm::vec3& scale)
{
	transform.scale = scale;
	updateModelMat();

	/*for (size_t i = 0; i < children.getSize(); i++)
	{
		children[i].setScale(scale);
	}*/
}

const Transform& BaseObject::getTransform() const
{
	return transform;
}

const glm::mat4 BaseObject::getGlobalModelMat()
{
	//add flag if the matrix should be recalculated so we dont calculate it each time
	//dont calculate here the model matrix
	updateModelMat();
	return transform.modelMatrix;
}

const glm::mat4 BaseObject::globalToLocalMat(const glm::mat4& matrix) const
{
	return parentPtr ? glm::inverse(parentPtr->getGlobalModelMat()) * matrix : matrix;
}

void BaseObject::setParent(BaseObject* parent)
{
	parentPtr = parent;
}

glm::mat4 BaseObject::getTransformMat() const
{
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), transform.rotation.x, { 1,0,0 })
		* glm::rotate(glm::mat4(1.0f), transform.rotation.y, { 0,1,0 })
		* glm::rotate(glm::mat4(1.0f), transform.rotation.z, { 0,0,1 });

	return glm::translate(glm::mat4(1.0f), transform.position)
		* rotation
		* glm::scale(glm::mat4(1.0f), transform.scale);
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

void BaseObject::setTransform(const glm::mat4& transformMat)
{
	//A lot of calculations here, consider a better way
	glm::vec3 rotation;
	Engine::Math::DecomposeMatrix(transformMat, transform.position, rotation, transform.scale);

	transform.rotation.x = glm::degrees(rotation.x);
	transform.rotation.y = glm::degrees(rotation.y);
	transform.rotation.z = glm::degrees(rotation.z);
}

void BaseObject::updateModelMat()
{
	//update the matrix with the current transform
	transform.modelMatrix = glm::translate(glm::mat4(1.0f), transform.position);

	//THE ROTATION SHOULD BE APPLIED IN ZYX ORDER
	transform.modelMatrix = glm::rotate(transform.modelMatrix, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transform.modelMatrix = glm::rotate(transform.modelMatrix, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform.modelMatrix = glm::rotate(transform.modelMatrix, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	transform.modelMatrix = glm::scale(transform.modelMatrix, transform.scale);

	//multiply by the parents matrix, makes the matrix with global coordinates
	transform.modelMatrix = parentPtr ? parentPtr->getGlobalModelMat() * transform.modelMatrix : transform.modelMatrix;
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
	children[index]->parentPtr = nullptr;
	children.erase(children.begin() + index);
}

void BaseObject::removeChild(BaseObject* object)
{
	unsigned childrenCount = children.size();
	for (size_t i = 0; i < childrenCount; i++)
	{
		if(object == children[i])
		{
			children[i]->parentPtr = nullptr;
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

	glm::mat4 globalMat = getGlobalModelMat();

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

	setTransform(globalToLocalMat(globalMat));
}

//Check if this object is a direct or indirect child of the given object
bool BaseObject::isChildOf(const BaseObject* object) const
{
	//bottom of recursion
	if(!parentPtr)
	{
		return false;
	}

	//return true if the parent is the one we are looknig for
	if(parentPtr == object)
	{
		return true;
	}

	//go up the tree
	return parentPtr->isChildOf(object);
}

void BaseObject::draw() const
{
	unsigned meshesCount = children.size();
	for (size_t i = 0; i < meshesCount; i++)
	{
		children[i]->draw();
	}
}