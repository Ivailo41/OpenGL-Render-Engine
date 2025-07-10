#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned>& indices, const std::string& name, const std::vector<MaterialGroup>& matGroups) : BaseObject()
{
	generateBuffers(VAO, VBO, EBO);
	initialize(verts, indices);
	setName(name);
	materialGroups = matGroups;
}

Mesh::~Mesh() //EACH COPY WILL SHARE ONE VERTEX BUFFER SO USE A SHARED POINTER AND AFTER ALL INSTANCES ARE DELETED, DELETE THE BUFFER
{
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
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

	//glBindVertexArray(0);
}

void Mesh::draw(Shader* overrideShader, GLenum drawMode) const
{
	BaseObject::draw(overrideShader);

	if(vertices.size() == 0 || vIndices.size() == 0)
	{
		return;
	}

	for (auto materialGroup : materialGroups)
	{
		glBindVertexArray(VAO);

		//this binds the shader asigned to the material
		if (overrideShader != nullptr)
		{
			overrideShader->setMat4("transform", transform.modelMatrix);
			//overrideShader->use();
		}
		else
		{
			unsigned shaderProgram = materialGroup.material->getShaderProgram();
			glUseProgram(shaderProgram);
			materialGroup.material->getShader()->setMat4("transform", transform.modelMatrix);

			//this might go to the pbr material class
			if(materialGroup.material != nullptr)
			{
				materialGroup.material->sendToShader();
			}
		}

		glDrawElements(drawMode, materialGroup.indicesCount, GL_UNSIGNED_INT, (void*)(materialGroup.offset * sizeof(unsigned)));
		glBindVertexArray(0);
	}
}

void Mesh::update(float deltaTime)
{

}
