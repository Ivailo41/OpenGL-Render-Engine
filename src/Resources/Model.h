#pragma once
#include <memory>

#include "Mesh.h"
#include "Raw/RawStructs.h"

class Model {
public:
    Model(const RawModel& rawModel, const std::unordered_map<std::string, Material>& materialsList);

    const std::vector<std::unique_ptr<Mesh>>& getMeshes() const { return this->meshes; }
    const RawModelNode& getRoot() const { return this->root; }
    const std::string& getName() const { return this->name; }

private:
    std::vector<std::unique_ptr<Mesh>> meshes;
    RawModelNode root;
    std::string name;
};
