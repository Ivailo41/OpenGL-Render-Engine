#include "ResourceManager.h"

void ResourceManager::loadTexture(const std::filesystem::path& path) {

}

void ResourceManager::loadShader(const std::string& name, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::filesystem::path& geometryPath = std::filesystem::path{}) {
    const std::vector<std::string> shaderSources = fileManager.loadShader(vertexPath, fragmentPath, geometryPath);

    if(shaderSources.size() != 3) {
        //log error or throw later
        return;
    }
    Shader shader(name, shaderSources[0], shaderSources[1], shaderSources[2]);
    shaders.emplace(name, shader);
}

bool ResourceManager::loadModel(const std::filesystem::path& path) {

    try {
        models.emplace(path.filename(), Model(fileManager.loadOBJ(path)));
    }
    catch (std::runtime_error& e) {
        return false;
    }

    return true;
}

ResourceManager::ResourceManager(const FileManager &fileManager) : fileManager(fileManager) {
    //nothing to do here
}

