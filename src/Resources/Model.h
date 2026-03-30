#pragma once
#include "Mesh.h"
#include "Raw/RawStructs.h"

class Model {
public:
    Model(const RawModel& rawModel);

private:
    std::vector<Mesh> meshes;
    RawModelNode root;
    std::string name;
};
