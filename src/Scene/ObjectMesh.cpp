#include "ObjectMesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

ObjectMesh::ObjectMesh(const std::string& name, const Mesh* mesh) : BaseObject()
{
	setName(name);
	this->mesh = mesh;
}

ObjectMesh::~ObjectMesh() //EACH COPY WILL SHARE ONE VERTEX BUFFER SO USE A SHARED POINTER AND AFTER ALL INSTANCES ARE DELETED, DELETE THE BUFFER
{
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
}

void ObjectMesh::draw(Shader* overrideShader, GLenum drawMode) const
{
	BaseObject::draw(overrideShader);

	/*if(vertices.size() == 0 || vIndices.size() == 0)
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
	}*/
}

void ObjectMesh::update(float deltaTime)
{
	BaseObject::update(deltaTime);
}
