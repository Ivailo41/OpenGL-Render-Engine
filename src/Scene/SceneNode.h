#pragma once

/*#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>*/

#define GLM_ENABLE_EXPERIMENTAL
#include "gtx/matrix_decompose.hpp"

#include "../Renderer/LinesContainer.h"

#include "TransformComponent.h"
#include "MeshComponent.h"

class SceneNode
{
public:
	void setName(const std::string& name);
	std::string getName() const { return name; }

	const glm::mat4 getGlobalModelMat() const;
	const glm::mat4 globalToLocalMat(const glm::mat4& matrix) const;

	virtual void draw(Shader* overrideShader = nullptr, GLenum drawMode = GL_TRIANGLES) const;
	virtual void update(float deltaTime);
	virtual SceneNode* clone() const { return new SceneNode(*this); }

	void setParent(SceneNode* parent);
	SceneNode* getParent() const { return parentPtr; }

	unsigned getChildrenCount() const { return children.size(); }
	void attachTo(SceneNode* parent);

	bool isChildOf(const SceneNode* object) const;

	template<typename T, typename... Args>
	T* addComponent(Args&&... args);

	template<typename T>
	T* getComponent() const;

public:
	SceneNode();
	SceneNode(const std::string& name);
	virtual ~SceneNode();

public:
	void addChild(SceneNode& child);
	void addChild(SceneNode* child);

	//doesnt delete the object!!!!
	void removeChild(unsigned index);
	void removeChild(SceneNode* object);

	//test
	void drawDebug() const;

	LinesContainer debugLinesContainer;
	std::vector<SceneNode*> children;

protected:
	std::string name;
	SceneNode* parentPtr;

	TransformComponent* transformComponentPtr;
	MeshComponent* meshComponentPtr;
};

template<>
inline TransformComponent* SceneNode::addComponent<TransformComponent>()
{
	if (!meshComponentPtr)
	{
		transformComponentPtr = new TransformComponent();
	}
	return transformComponentPtr;
}

template<>
inline MeshComponent* SceneNode::addComponent<MeshComponent>()
{
	if(!meshComponentPtr)
	{
		meshComponentPtr = new MeshComponent();
	}
	return meshComponentPtr;
}

template<>
inline TransformComponent* SceneNode::getComponent<TransformComponent>() const
{
	return transformComponentPtr;
}

template<>
inline MeshComponent* SceneNode::getComponent<MeshComponent>() const
{
	return meshComponentPtr;
}
