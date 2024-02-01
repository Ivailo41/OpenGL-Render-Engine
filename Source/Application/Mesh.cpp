#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned>& indices) : BaseObject()
{
	generateBuffers(VAO, VBO, EBO);
	initialize(verts, indices);
}

Mesh::Mesh(const Mesh& other) : BaseObject(other)
{
	vertices = other.vertices;
	vIndices = other.vIndices;
	material = other.material;
	generateBuffers(VAO, VBO, EBO);
	initialize(vertices, vIndices);
}

Mesh& Mesh::operator=(const Mesh& other)
{
	if(this != &other)
	{
		BaseObject::operator=(other);
		vertices = other.vertices;
		vIndices = other.vIndices;
		material = other.material;
		generateBuffers(VAO, VBO, EBO);
		initialize(vertices, vIndices);
	}
	return *this;
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::generateBuffers(unsigned& VAO, unsigned& VBO, unsigned& EBO)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void Mesh::initialize(const std::vector<Vertex>& verts, const std::vector<unsigned>& indices)
{
	vertices = verts;
	vIndices = indices;

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndices.size() * sizeof(unsigned), &vIndices[0], GL_STATIC_DRAW);

	// glVertexAttribPointer(index of attribute, number of components, type of data, normalize?, total size of one vertex, offset from where to start reading)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void Mesh::draw() const
{
	BaseObject::draw();

	if(vertices.size() == 0 || vIndices.size() == 0)
	{
		return;
	}
	unsigned shaderProgram = material->getShaderProgram();

	unsigned int transformLoc = glGetUniformLocation(material->getShaderProgram(), "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glBindVertexArray(VAO);
	if(material != nullptr)
	{
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, material->operator[](0).id);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, material->operator[](1).id);
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, material->operator[](2).id);
	}
	glDrawElements(GL_TRIANGLES, vIndices.size(), GL_UNSIGNED_INT, nullptr);
}

void Mesh::setMaterial(Material* const material)
{
	this->material = material;
}
