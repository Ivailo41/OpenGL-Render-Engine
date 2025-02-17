#pragma once

#include "Primitives.h"
#include <string>
#include "BaseObject.h"
#include "Texture.h"
#include "Material.h"

class Mesh : public BaseObject
{
public:
	void draw() const override;
	BaseObject* clone() const override { return new Mesh(*this); }

	unsigned getVBO() const { return VBO; }
	unsigned getEBO() const { return EBO; }

	//not used
	glm::mat4 getModelMatrix() const { return transform.modelMatrix; }

	void setMaterial(Material* const material);

public:
	Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned>& indices);
	//Mesh(const Mesh& other);
	//Mesh& operator=(const Mesh& other);
	virtual ~Mesh();

protected:
	void generateBuffers(unsigned& VAO, unsigned& VBO, unsigned& EBO);
	void initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices);

protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned> vIndices;

	Material* material;

	unsigned VAO, VBO, EBO;

private:
	Mesh();
};