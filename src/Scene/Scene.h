#pragma once
#include "Camera.h"
#include "BaseObject.h"
#include "Lights/Light.h"
#include <string>
#include <vector>
#include "Skybox.h"

class Scene
{
public:
	//bool loadObject(const std::string& path);

	bool addObject(BaseObject* object);
	void removeObject(BaseObject* object);

	BaseObject* getSelectedObject() const;
	void setSelectedObject(BaseObject* object);

	//remove that draw function
	void updateObjects(float deltaTime);

	void setName(const std::string& name);
	std::string getName() const;

	void setActiveCamera(Camera* camera);
	Camera* getActiveCamera() const { return activeCamera; }

	//void clear();

public:
	Scene();
	Scene(const std::string& sceneName);
	~Scene();

public:
	static Scene* activeScene;
	//std::vector<BaseObject*> sceneObjects;
	BaseObject root;

	std::vector<Texture*> textures;
	std::vector<Material*> materials;
	//add container for cameras
	std::vector<Light*> lights;

	Skybox* activeSkybox = nullptr;

private:
	std::string name;

	//might move these pointers elsewhere
	Camera* activeCamera;
	BaseObject* selectedObject;
};

