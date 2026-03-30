#include "Model.h"

Model::Model(const RawModel &rawModel) {
    this->root = rawModel.root;
    this->name = rawModel.name;

    for (const auto&[vertices, indices, name, materialGroups] : rawModel.meshes) {
        this->meshes.push_back(Mesh(vertices, indices, name, materialGroups));
    }
}
