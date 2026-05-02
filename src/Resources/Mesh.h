#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../Renderer/Primitives.h"
#include "Raw/RawStructs.h"
#include <vector>

#include "Material.h"

struct MaterialGroup
{
    unsigned offset;
    unsigned indicesCount;
    const Material* material;

    MaterialGroup() : offset(0), indicesCount(0) {}
};

class Mesh {

public:
    const std::string& getName() const { return name; }
    const std::vector<MaterialGroup>& getMaterialGroups() const { return materialGroups; }
    unsigned getVAO() const { return VAO; }

public:
    Mesh(const RawMesh& rawMesh, const std::unordered_map<std::string, Material>& materialsList);
    ~Mesh();

private:
    void generateBuffers();
    void initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices) const;

private:
    std::string name;
    std::vector<MaterialGroup> materialGroups;
    unsigned VAO, VBO, EBO;
};
