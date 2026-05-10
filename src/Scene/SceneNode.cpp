#include "SceneNode.h"

SceneNode::SceneNode() : parentPtr(nullptr), transformComponentPtr(nullptr), meshComponentPtr(nullptr), cameraComponentPtr(nullptr)
{
	name = "New Object";
	addComponent<TransformComponent>();
}

SceneNode::SceneNode(const std::string& name) : parentPtr(nullptr), transformComponentPtr(nullptr), meshComponentPtr(nullptr), cameraComponentPtr(nullptr)
{
	this->name = name;
	addComponent<TransformComponent>();
}

SceneNode::~SceneNode()
{
	if(transformComponentPtr)
		delete transformComponentPtr;
	if(meshComponentPtr)
		delete meshComponentPtr;
}

void SceneNode::setName(const std::string& name)
{
	this->name = name;
}

const glm::mat4 SceneNode::getGlobalModelMat() const
{
	return parentPtr ? parentPtr->getGlobalModelMat() * transformComponentPtr->getModelMatrix() : transformComponentPtr->getModelMatrix();
}

const glm::mat4 SceneNode::globalToLocalMat(const glm::mat4& matrix) const
{
	return parentPtr ? glm::inverse(parentPtr->getGlobalModelMat()) * matrix : matrix;
}

void SceneNode::setParent(SceneNode* parent)
{
	parentPtr = parent;
}

void SceneNode::addChild(SceneNode& child)
{
	children.push_back(&child);
	child.parentPtr = this;
}

void SceneNode::addChild(SceneNode* child)
{
	child->parentPtr = this;
	children.push_back(child);
}

void SceneNode::removeChild(unsigned index)
{
	children[index]->parentPtr = nullptr;
	children.erase(children.begin() + index);
}

void SceneNode::removeChild(SceneNode* object)
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
void SceneNode::drawDebug() const
{
	if(!debugLinesContainer.isEmpty())
	{
		debugLinesContainer.drawLines(transformComponentPtr->getModelMatrix());
	}

	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->drawDebug();
	}
}

void SceneNode::attachTo(SceneNode* parent)
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

	transformComponentPtr->setTransform(globalToLocalMat(globalMat));
}

//Check if this object is a direct or indirect child of the given object
bool SceneNode::isChildOf(const SceneNode* object) const
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

void SceneNode::draw(Shader* overrideShader, GLenum drawMode) const
{

	if(meshComponentPtr)
	{
		meshComponentPtr->draw(getGlobalModelMat(), overrideShader, drawMode);
	}

	unsigned meshesCount = children.size();
	for (size_t i = 0; i < meshesCount; i++)
	{
		children[i]->draw(overrideShader, drawMode);
	}
}

void SceneNode::update(float deltaTime)
{
	unsigned meshesCount = children.size();
	for (size_t i = 0; i < meshesCount; i++)
	{
		children[i]->update(deltaTime);
	}
}
