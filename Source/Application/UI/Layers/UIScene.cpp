#include "UIScene.h"
#include "../Elements/UISceneNode.h"

UIScene::UIScene() : UILayer("Scene")
{

}

UIScene::UIScene(const char* layerName) : UILayer(layerName)
{

}

void UIScene::renderLayer()
{
	unsigned childCount = Scene::activeScene->sceneObjects.getSize();
	ImGui::Begin(layerName.c_str());
	if (ImGui::TreeNode("Scene"))
	{
		for (size_t i = 0; i < childCount; i++)
		{
			BaseObject* child = &Scene::activeScene->sceneObjects[i];
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

UIScene* UIScene::clone()
{
	return new UIScene(*this);
}