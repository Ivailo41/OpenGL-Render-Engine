#include "UISceneNode.h"
#include "../../Editor/Scene.h"

UISceneNode::UISceneNode(const std::string& name, BaseObject* object) : UIElement(name), object(object)
{
}

UISceneNode::~UISceneNode()
{
}

void UISceneNode::renderElement()
{
	if(ImGui::TreeNode(object->getName().c_str()))
	{
		unsigned childCount = object->getChildrenCount();
		for (size_t i = 0; i < childCount; i++)
		{
			BaseObject* child = &object->operator[](i);
			if (child != nullptr)
			{
				UISceneNode* node = new UISceneNode(child->getName(), child);
				node->renderElement();
				delete node;
			}
		}

		ImGui::TreePop();
	}

	if (ImGui::IsItemClicked())
	{
		Scene::activeScene->setSelectedObject(object);
	}
	else if (ImGui::IsItemClicked(1))
	{
		//object->getParent()->removeChild(object);
		//Scene::activeScene->removeObject(object);
	}

	if (ImGui::BeginDragDropSource())
	{
		
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
		{
			
		}
		ImGui::EndDragDropTarget();
	}
}
