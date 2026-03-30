#include "ResourceManager.h"

#include "stb_image.h"

void ResourceManager::loadTexture(const std::vector<std::filesystem::path>& paths) {
    std::vector<RawTexture> rawTextures = fileManager.loadTextures(paths);

    for (int i = 0; i < rawTextures.size(); ++i) {
        textures.emplace(paths[i], Texture(rawTextures[i].data, rawTextures[i].width, rawTextures[i].height, paths[i].c_str()));
        //free the image data loaded with stbi
        stbi_image_free((rawTextures[i].data));
    }
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

    RawModel rawModel;

    try {
        rawModel = fileManager.loadOBJ(path);
    }
    catch (std::runtime_error& e) {
        return false;
    }

    //There might be better way of loading the unique materials
    for (auto& rawMesh : rawModel.meshes) {
        for(auto& materialGroup : rawMesh.rawMaterialGroups) {
            materials.insert({materialGroup.materialName, Material(materialGroup.materialName)});
        }
    }

    models.emplace(path.filename(), Model(rawModel, materials));

    LOG_TRACE("Loaded model: " + path.string());
    return true;
}

ResourceManager::ResourceManager(const FileManager &fileManager) : fileManager(fileManager) {
    //nothing to do here
}

const Model* ResourceManager::getModel(const std::string& name) const {
    auto it = models.find(name);
    if (it == models.end()) {
        return nullptr;
    }
    return &it->second;
}

