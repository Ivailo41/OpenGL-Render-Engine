#include "Scene.h"
#include "../FileManager.h"

Scene* Scene::activeScene = nullptr;

Scene::Scene()
{
	name = "New Scene";
}

void Scene::loadObject(const std::string& path)
{
	//change later 
	Object* object = FileManager::readOBJ(path);
	if (object != nullptr)
	{
		sceneObjects.push_back(object);
	}
}

void Scene::setSelectedObject(BaseObject* object)
{
	selectedObject = object;
}

void Scene::drawObjects() const
{
	for (size_t i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->draw();
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

//OLD CODE

//void Scene::removeObject(unsigned index)
//{
//	sceneObjects.removeObject(index);
//}

void Scene::removeObject(BaseObject* object)
{
	if (selectedObject == object)
	{
		selectedObject = nullptr;
	}
	for (size_t i = 0; i < sceneObjects.size(); i++)
	{
		if(sceneObjects[i] == object)
		{
			sceneObjects.erase(sceneObjects.begin() + i);
			delete object;
			return;
		}
	}
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