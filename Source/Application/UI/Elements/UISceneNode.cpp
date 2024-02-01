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
	if (ImGui::IsItemClicked(0))
	{
		Scene::activeScene->setSelectedObject(object);
	}
	else if (ImGui::IsItemClicked(1))
	{
		//object remove
	}

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

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("DND_DEMO_CELL", &object, sizeof(object)); ///here
		ImGui::Text("Drag and drop me");
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
		{
			////IM_ASSERT(payload->DataSize == sizeof(*object));
			//BaseObject** draggedObject = (BaseObject**)payload->Data;
			////Scene::activeScene->setSelectedObject(draggedObject->attachTo(*object));
			//BaseObject* temp = object;
			//object = *draggedObject;
			//*draggedObject = temp;

			BaseObject** draggedObj = (BaseObject**)payload->Data;
			BaseObject* temp = *draggedObj;
			*draggedObj = object;
			object = temp;
		}
		ImGui::EndDragDropTarget();
	}
}
