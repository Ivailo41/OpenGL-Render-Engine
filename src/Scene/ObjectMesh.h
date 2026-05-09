#pragma once

#include <string>
#include "SceneNode.h"
#include "../Resources/Material.h"
#include "../Resources/Mesh.h"

class ObjectMesh : public SceneNode
{
public:
	void draw(Shader* overrideShader = nullptr, GLenum drawMode = GL_TRIANGLES) const override;
	void update(float deltaTime) override;
	SceneNode* clone() const override { return new ObjectMesh(*this); }

public:
	ObjectMesh(const std::string& name, const Mesh* mesh);
	~ObjectMesh() override = default;

protected:
	const Mesh* mesh = nullptr;
};