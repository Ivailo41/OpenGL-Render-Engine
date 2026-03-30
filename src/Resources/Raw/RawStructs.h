#pragma once
#include <string>
#include <vector>
#include "../../Renderer/Primitives.h"

struct RawMaterialGroup
{
    unsigned offset;
    unsigned indicesCount;
    std::string materialName;

    RawMaterialGroup() : offset(0), indicesCount(0) {}
};

struct RawMesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::string name;
    std::vector<RawMaterialGroup> rawMaterialGroups;
};

struct RawModelNode {
    std::vector<unsigned> meshIndices;
    std::vector<RawModelNode> children;
};

struct RawModel {
    std::vector<RawMesh> meshes;
    RawModelNode root;
    std::string name;
};

struct RawTexture {
    unsigned char* data;
    int width, height, nrChannels;
};
