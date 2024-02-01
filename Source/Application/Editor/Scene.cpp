#include "Scene.h"
#include "../FileManager.h"

Scene* Scene::activeScene = nullptr;

Scene::Scene()
{
	name = "New Scene";
}

//Scene::Scene(const Scene& other)
//{
//	name = other.name;
//	sceneObjects = other.sceneObjects;
//}
//
//Scene& Scene::operator=(const Scene& other)
//{
//	if (this != &other)
//	{
//		name = other.name;
//		sceneObjects = other.sceneObjects;
//	}
//	return *this;
//}

//Scene::~Scene()
//{
//	for (size_t i = 0; i < sceneObjects.getSize(); i++)
//	{
//		delete& sceneObjects[i];
//	}
//}

BaseObject* Scene::loadObject(const std::string& path)
{
	//change later 
	Object* object = FileManager::readOBJ(path);
	if (object != nullptr)
	{
		BaseObject* loadedObject = sceneObjects.addObject(*object);
		delete object;
		return loadedObject;
	}
	return nullptr;
}

void Scene::setSelectedObject(BaseObject* object)
{
	selectedObject = object;
}

void Scene::drawObjects() const
{
	for (size_t i = 0; i < sceneObjects.getSize(); i++)
	{
		sceneObjects[i].draw();
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
	sceneObjects.removeObject(object);
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