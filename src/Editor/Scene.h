#pragma once
#include "../Camera.h"
#include "../BaseObject.h"
#include "../Object.h"
#include "../Lights/Light.h"
#include <string>
#include <vector>

class Scene
{
public:
	//bool loadObject(const std::string& path);

	bool addObject(BaseObject* object);
	void removeObject(BaseObject* object);

	BaseObject* getSelectedObject() const;
	void setSelectedObject(BaseObject* object);

	void drawObjects() const;

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
	std::vector<BaseObject*> sceneObjects;

	std::vector<Texture*> textures;
	std::vector<Material*> materials;
	//add container for cameras
	//add container for lights

private:
	std::string name;

	//might move these pointers elsewhere
	Camera* activeCamera;
	BaseObject* selectedObject;
};

