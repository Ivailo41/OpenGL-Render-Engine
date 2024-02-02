#include "UISceneTree.h"
#include "../Elements/UISceneNode.h"

UISceneTree::UISceneTree() : UILayer("Scene Tree")
{

}

UISceneTree::UISceneTree(const char* layerName) : UILayer(layerName)
{

}

void UISceneTree::renderLayer()
{
	ImGui::Begin(layerName.c_str());
	if (ImGui::TreeNode("Scene Tree"))
	{
		for (size_t i = 0; i < Scene::activeScene->sceneObjects.size(); i++)
		{
			BaseObject* child = Scene::activeScene->sceneObjects[i];
			if (child != nullptr)
			{
				UISceneNode* node = new UISceneNode(child->getName(), child);
				node->renderElement();
				delete node;
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();

}

UISceneTree* UISceneTree::clone()
{
	return new UISceneTree(*this);
}