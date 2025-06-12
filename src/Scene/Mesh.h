#pragma once

#include "../Renderer/Primitives.h"
#include <string>
#include "BaseObject.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Material.h"

struct MaterialGroup
{
	unsigned offset;
	unsigned indicesCount;
	const Material* material = nullptr;

	MaterialGroup() : offset(0), indicesCount(0) {}
};

class Mesh : public BaseObject
{
public:
	void draw() const override;
	BaseObject* clone() const override { return new Mesh(*this); }

	unsigned getVBO() const { return VBO; }
	unsigned getEBO() const { return EBO; }

public:
	Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned>& indices, const std::string& name, const std::vector<MaterialGroup>& matGroups);
	virtual ~Mesh();

protected:
	void generateBuffers(unsigned& VAO, unsigned& VBO, unsigned& EBO);
	void initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices);

protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned> vIndices;

	std::vector<MaterialGroup> materialGroups;

	//Material* material;

	unsigned VAO, VBO, EBO;

private:
	Mesh();
};