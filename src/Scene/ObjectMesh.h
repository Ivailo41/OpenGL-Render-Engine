#pragma once

#include "../Renderer/Primitives.h"
#include <string>
#include "BaseObject.h"
#include "../Resources/Texture.h"
#include "../Resources/Material.h"
#include "../Resources/Mesh.h"

class ObjectMesh : public BaseObject
{
public:
	void draw(Shader* overrideShader = nullptr, GLenum drawMode = GL_TRIANGLES) const override;
	void update(float deltaTime) override;
	BaseObject* clone() const override { return new ObjectMesh(*this); }

public:
	ObjectMesh(const std::string& name, const Mesh* mesh);
	~ObjectMesh() override = default;

protected:
	const Mesh* mesh = nullptr;
};