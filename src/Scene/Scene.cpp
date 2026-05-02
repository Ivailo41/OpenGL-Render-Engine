#include "Scene.h"
#include <queue>

Scene* Scene::activeScene = nullptr;

Scene::Scene(const ResourceManager& resourceManager) : name("New Scene"), activeCamera(nullptr), selectedObject(nullptr),  root("Scene"), resourceManager(resourceManager)
{
	//nothing to do here
}

Scene::Scene(const std::string& sceneName, const ResourceManager& resourceManager) : name(sceneName), activeCamera(nullptr), selectedObject(nullptr), root("Scene"), resourceManager(resourceManager)
{
	//nothing to do here
}

Scene::~Scene()
{
	//unsigned sceneObjectsCount = sceneObjects.size();
	unsigned sceneObjectsCount = root.getChildrenCount();
	for (size_t i = 0; i < sceneObjectsCount; i++)
	{
		//delete sceneObjects[i];
	}
}

void Scene::setSelectedObject(BaseObject* object)
{
	selectedObject = object;
}

void Scene::updateObjects(float deltaTime)
{
	root.update(deltaTime);
}

void Scene::setName(const std::string& name)
{
	this->name = name;
}

std::string Scene::getName() const
{
	return name;
}

void Scene::setActiveCamera(Camera* camera)
{
	activeCamera = camera;
}

bool Scene::instanceModel(const std::string& modelName)
{
	const Model* model = resourceManager.getModel(modelName);
	if (model == nullptr) {
		LOG_ERROR("Scene could not find model \"" + modelName + "\"");
		return false;
	}

	auto object = new BaseObject(model->getName());
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
				const auto objectMesh = new ObjectMesh(meshes[index]->getName(), meshes[index].get()); //Carefully with mesh.get()
				objectMesh->attachTo(object);
			}
		}
	}

	root.addChild(object);
	return true;
}

bool Scene::addObject(BaseObject *object) {

	if (object == nullptr)
		return false;

	//sceneObjects.push_back(object);
	Light* light = dynamic_cast<Light*>(object);
	if (light != nullptr)
	{
		lights.push_back(light);
	}

	root.addChild(object);
	return true;
}

void Scene::removeObject(BaseObject* object)
{
	if (object == nullptr)
		return;

	Light* light = dynamic_cast<Light*>(object);
	if (light != nullptr)
	{
		for (size_t i = 0; i < lights.size(); i++)
		{
			if (lights[i] == light)
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

	root.removeChild(object);
}
	
BaseObject* Scene::getSelectedObject() const
{
	return selectedObject;
}