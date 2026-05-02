#include "Mesh.h"

Mesh::Mesh(const RawMesh& rawMesh, const std::unordered_map<std::string, Material>& materialsList)
: VAO(0), VBO(0), EBO(0), materialGroups(rawMesh.rawMaterialGroups.size()) {
    generateBuffers();
    initialize(rawMesh.vertices, rawMesh.indices);
    name = rawMesh.name;

    for (int i = 0; i < materialGroups.size(); i++) {
        materialGroups[i].indicesCount = rawMesh.rawMaterialGroups[i].indicesCount;
        materialGroups[i].offset = rawMesh.rawMaterialGroups[i].offset;
        materialGroups[i].material = &materialsList.find(rawMesh.rawMaterialGroups[i].materialName)->second;
    }
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::generateBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void Mesh::initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices) const
{
    //vertices = verts;
    //vIndices = indices;

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

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