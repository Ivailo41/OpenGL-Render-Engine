#include "Model.h"

Model::Model(const RawModel &rawModel, const std::unordered_map<std::string, Material>& materialsList) {
    this->root = rawModel.root;
    this->name = rawModel.name;

    for (const auto& rawMesh: rawModel.meshes) {
        this->meshes.push_back(std::make_unique<Mesh>(rawMesh, materialsList));
    }
}
