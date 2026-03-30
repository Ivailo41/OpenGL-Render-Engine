#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../Renderer/Primitives.h"
#include "Raw/RawStructs.h"
#include <vector>

class Mesh {

public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices, const std::string& name, const std::vector<MaterialGroup>& matGroups);
    ~Mesh();

private:
    void generateBuffers();
    void initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices) const;

private:
    std::string name;
    std::vector<MaterialGroup> materialGroups;
    unsigned VAO, VBO, EBO;
};
