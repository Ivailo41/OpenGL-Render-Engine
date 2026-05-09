#pragma once
#include <string>
#include "../Resources/Mesh.h"

class MeshComponent
{
public:
	MeshComponent() = default;
	MeshComponent(const Mesh* mesh);

	const Mesh* getMesh() const { return mesh; }
	void setMesh(const Mesh* newMesh) { mesh = newMesh; }

	//TEMP UNTILL THE RENDERING IS PASSED TO THE RENDERER
	void draw(const glm::mat4& globalMat, Shader* overrideShader = nullptr, GLenum drawMode = GL_TRIANGLES);

protected:
	const Mesh* mesh = nullptr;
};