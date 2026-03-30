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

	//unsigned getVBO() const { return VBO; }
	//unsigned getEBO() const { return EBO; }

public:
	ObjectMesh(const std::string& name, const Mesh* mesh);
	virtual ~ObjectMesh();

protected:
	//void generateBuffers(unsigned& VAO, unsigned& VBO, unsigned& EBO);
	//void initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices);

protected:
	//std::vector<Vertex> vertices;
	//std::vector<unsigned> vIndices;
	//std::vector<MaterialGroup> materialGroups;
	const Mesh* mesh = nullptr;

	//unsigned VAO, VBO, EBO;
};