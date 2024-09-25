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
	bool loadObject(const std::string& path);

	void addObject(BaseObject* object);
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
	Scene(const Scene& other) = default;
	Scene& operator=(const Scene& other) = default;
	~Scene() = default;

public:
	static Scene* activeScene;
	std::vector<BaseObject*> sceneObjects;

	//add container for cameras
	//add container for lights

private:
	std::string name;
	Camera* activeCamera;
	BaseObject* selectedObject;
};

