#include "Scene.h"
//#include "../FileManager.h"

Scene* Scene::activeScene = nullptr;

Scene::Scene() : name("New Scene"), activeCamera(nullptr), selectedObject(nullptr),  root("Scene")
{
	//nothing to do here
}

Scene::Scene(const std::string& sceneName) : name(sceneName), activeCamera(nullptr), selectedObject(nullptr), root("Scene")
{
	//nothing to do here
}

Scene::~Scene()
{
	//unsigned sceneOjectsCount = sceneObjects.size();
	unsigned sceneOjectsCount = root.getChildrenCount();
	for (size_t i = 0; i < sceneOjectsCount; i++)
	{
		//this will cause error trying to delete objects allocated on the stack
		//delete sceneObjects[i];
	}
}

//bool Scene::loadObject(const std::string& path)
//{
//
//	Object* object = FileManager::readOBJ(path);
//	if (object == nullptr)
//	{
//		return false;
//	}
//
//	sceneObjects.push_back(object);
//	return true;
//}

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

//OLD CODE

//void Scene::removeObject(unsigned index)
//{
//	sceneObjects.removeObject(index);
//}

bool Scene::addObject(BaseObject* object)
{
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

//BaseObject* Scene::addObject(BaseObject& object)
//{
//	return sceneObjects.addObject(object);
//}

//void Scene::addObject(BaseObject* object)
//{
//	sceneObjects.addObject(object);
//}