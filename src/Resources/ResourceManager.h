#pragma once
#include <unordered_map>
#include <filesystem>
#include <string>

#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "Model.h"

#include "../Core/FileManager.h"

class ResourceManager {

public:

    void loadTexture(const std::vector<std::filesystem::path>& path);
    void loadShader(const std::string& name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::filesystem::path& geometryPath);
    bool loadModel(const std::filesystem::path& path);

    const Texture* getTexture(const std::string& name) const;
    const Shader* getShader(const std::string& name) const;
    const Model* getModel(const std::string& name) const;
    const Material* getMaterial(const std::string& name) const;


public:
    ResourceManager(const FileManager& fileManager);

private:
    std::unordered_map<std::string, Texture> textures;
    std::unordered_map<std::string, Shader> shaders;
    std::unordered_map<std::string, Model> models;

    std::unordered_map<std::string, Material> materials;

    const FileManager& fileManager;
};