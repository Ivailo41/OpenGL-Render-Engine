#include "Scene.h"
#include <queue>

Scene* Scene::activeScene = nullptr;

Scene::Scene(const ResourceManager& resourceManager) : name("New Scene"), activeCamera(nullptr), selectedObject(nullptr), resourceManager(resourceManager)
{
	//nothing to do here
}

Scene::Scene(const std::string& sceneName, const ResourceManager& resourceManager) : name(sceneName), activeCamera(nullptr), selectedObject(nullptr), resourceManager(resourceManager)
{
	//nothing to do here
}

Scene::~Scene()
{
	for(SceneNode* object : sceneObjects)
	{
		delete object;
	}
}

void Scene::setSelectedObject(SceneNode* object)
{
	selectedObject = object;
}

void Scene::updateObjects(float deltaTime)
{
	//root.update(deltaTime);
	for(SceneNode* object : sceneObjects)
	{
		object->update(deltaTime);
	}
}

void Scene::setName(const std::string& name)
{
	this->name = name;
}

std::string Scene::getName() const
{
	return name;
}

void Scene::setActiveCamera(SceneNode* camera)
{
	if (camera->getComponent<CameraComponent>()) {
		activeCamera = camera;
	}
}

bool Scene::instanceModel(const std::string& modelName)
{
	const Model* model = resourceManager.getModel(modelName);
	if (model == nullptr) {
		LOG_ERROR("Scene could not find model \"" + modelName + "\"");
		return false;
	}

	auto sceneNode = new SceneNode(model->getName());
	const std::vector<std::unique_ptr<Mesh>>& meshes = model->getMeshes();

	std::queue<RawModelNode> nodesQueue;
	nodesQueue.push(model->getRoot());

	//This BFS implementation skips adding mesh for the root node but as the models are implemented they don't have meshes at root
	//This could be changed by making RawModelNode store vector of children instead root.
	while (!nodesQueue.empty()) {
		RawModelNode currentNode = nodesQueue.front();
		nodesQueue.pop();

		for (const RawModelNode& child : currentNode.children) {
			//make mesh and attach to object
			nodesQueue.push(child);
			for (unsigned index : child.meshIndices) {
				const auto meshNode = new SceneNode(meshes[index]->getName()); //Carefully with mesh.get()
				meshNode->addComponent<MeshComponent>();
				meshNode->getComponent<MeshComponent>()->setMesh(meshes[index].get());
				meshNode->attachTo(sceneNode);
			}
		}
	}

	sceneObjects.push_back(sceneNode);
	return true;
}

bool Scene::addObject(SceneNode *object) {

	if (object == nullptr)
		return false;

	LightComponent* lightComp = object->getComponent<LightComponent>();
	if (lightComp != nullptr)
	{
		lights.push_back(object);
	}

	sceneObjects.push_back(object);
	return true;
}

void Scene::removeObject(SceneNode* object)
{
	if (object == nullptr)
		return;

	LightComponent* lightComp = object->getComponent<LightComponent>();
	if (lightComp != nullptr)
	{
		for (size_t i = 0; i < lights.size(); i++)
		{
			if (lights[i] == object)
			{
				lights.erase(lights.begin() + i);
				break;
			}
		}
	}

	if (selectedObject == object)
	{
		selectedObject = nullptr;
	}

	sceneObjects.erase(std::remove(sceneObjects.begin(), sceneObjects.end(), object), sceneObjects.end());
}
	
SceneNode* Scene::getSelectedObject() const
{
	return selectedObject;
}