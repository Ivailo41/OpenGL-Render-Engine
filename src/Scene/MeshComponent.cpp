#include "MeshComponent.h"


MeshComponent::MeshComponent(const Mesh* mesh) : mesh(mesh)
{
}

void MeshComponent::draw(const glm::mat4& globalMat, Shader* overrideShader, GLenum drawMode)
{
	for (auto materialGroup : mesh->getMaterialGroups())
	{
		glBindVertexArray(mesh->getVAO());

		//this binds the shader asigned to the material
		if (overrideShader != nullptr)
		{
			overrideShader->setMat4("transform", globalMat);
			//overrideShader->use();
		}
		else //We enter this else when we use pbr shader, passing the pbr shader as override will skip this part and break rendering, fix it
		{
			unsigned shaderProgram = materialGroup.material->getShaderProgram();
			glUseProgram(shaderProgram);
			materialGroup.material->getShader()->setMat4("transform", globalMat);

			//this might go to the pbr material class
			if (materialGroup.material != nullptr)
			{
				materialGroup.material->sendToShader();
			}

		}

		glDrawElements(drawMode, materialGroup.indicesCount, GL_UNSIGNED_INT, (void*)(materialGroup.offset * sizeof(unsigned)));
		glBindVertexArray(0);
	}
}
