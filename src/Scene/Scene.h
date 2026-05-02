#pragma once
#include "Camera.h"
#include "BaseObject.h"
#include "Lights/Light.h"
#include <string>
#include <vector>
#include "Skybox.h"
#include "../Resources/ResourceManager.h"

class Scene
{
public:
	bool instanceModel(const std::string& name);
	bool addObject(BaseObject* object);
	void removeObject(BaseObject* object);

	BaseObject* getSelectedObject() const;
	void setSelectedObject(BaseObject* object);

	void updateObjects(float deltaTime);

	void setName(const std::string& name);
	std::string getName() const;

	void setActiveCamera(Camera* camera);
	Camera* getActiveCamera() const { return activeCamera; }

	//void clear();

public:
	Scene(const ResourceManager& resourceManager);
	Scene(const std::string& sceneName, const ResourceManager& resourceManager);
	~Scene();

public:
	static Scene* activeScene;
	BaseObject root;
	//add container for cameras
	std::vector<Light*> lights;

	Skybox* activeSkybox = nullptr;

private:
	std::string name;
	Camera* activeCamera;
	BaseObject* selectedObject;

	const ResourceManager& resourceManager;
};

