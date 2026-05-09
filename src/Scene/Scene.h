#pragma once
#include "Camera.h"
#include "SceneNode.h"
#include "Lights/Light.h"
#include <string>
#include <vector>
#include "Skybox.h"
#include "../Resources/ResourceManager.h"

class Scene
{
public:
	bool instanceModel(const std::string& name);
	bool addObject(SceneNode* object);
	void removeObject(SceneNode* object);

	SceneNode* getSelectedObject() const;
	void setSelectedObject(SceneNode* object);

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
	//SceneNode root;
	std::vector<SceneNode*> sceneObjects;
	//add container for cameras
	std::vector<Light*> lights;

	Skybox* activeSkybox = nullptr;

private:
	std::string name;
	Camera* activeCamera;
	SceneNode* selectedObject;

	const ResourceManager& resourceManager;
};

