#include "UISceneNode.h"
#include "../../Scene/Scene.h"
#include <iostream>

UISceneNode::UISceneNode(const std::string& name, BaseObject* object) : UIElement(name), object(object)
{
}

UISceneNode::~UISceneNode()
{
}

void UISceneNode::renderElement()
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

	unsigned childCount = object->getChildrenCount();
	bool isLeaf = !childCount;

	if (isLeaf)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	flags |= (Scene::activeScene->getSelectedObject() == object) ? ImGuiTreeNodeFlags_Selected : 0; //Make better way to compare objects instead of their raw pointers

	//Leaf nodes always return true for isOpen
	bool isOpen = ImGui::TreeNodeEx((void*)object, flags, object->getName().c_str());

	if(isOpen)
	{
		for (size_t i = 0; i < childCount; i++)
		{
			BaseObject* child = object->children[i];
			if (child != nullptr)
			{
				UISceneNode node(child->getName(), child);
				node.renderElement();
			}
		}

		ImGui::TreePop();
	}

	if ( ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() && (isLeaf || !isOpen) )
	{
		Scene::activeScene->setSelectedObject(object);
		std::cout << "Left clicked on: " << object->getName() << std::endl;
	}

	else if ( ImGui::IsItemClicked(1) && !ImGui::IsItemToggledOpen() && (isLeaf || !isOpen))
	{
		/*if (object->getParent() != nullptr)
		{
			object->getParent()->removeChild(object);
		}
		else
		{
			Scene::activeScene->removeObject(object);
		}*/

		std::cout << "Right clicked on: " << object->getName() <<std::endl;
	}

	//Placeholder drag and drop, currently does nothing
	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("_TREENODE", &object, sizeof(object));
		ImGui::Text(object->getName().c_str());
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
		{
			BaseObject** droppedObject = (BaseObject**)payload->Data;
			std::cout << "Dropped: " << (*droppedObject)->getName() << " On: " << object->getName() << std::endl;
		}
		ImGui::EndDragDropTarget();
	}
}
