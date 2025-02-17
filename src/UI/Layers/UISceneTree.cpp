#include "UISceneTree.h"
#include "../Elements/UISceneNode.h"

UI_SceneTree::UI_SceneTree() : UILayer("Scene Tree")
{

}

UI_SceneTree::UI_SceneTree(const char* layerName) : UILayer(layerName)
{

}

void UI_SceneTree::renderLayer()
{
	ImGui::Begin(layerName.c_str());

	for (size_t i = 0; i < Scene::activeScene->sceneObjects.size(); i++)
	{
		BaseObject* child = Scene::activeScene->sceneObjects[i];
		if (child != nullptr)
		{
			UISceneNode node(child->getName(), child);
			node.renderElement();
		}
	}
	ImGui::End();

}

UI_SceneTree* UI_SceneTree::clone()
{
	return new UI_SceneTree(*this);
}