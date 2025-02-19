#include "UISceneTree.h"

UI_SceneTree::UI_SceneTree() : UILayer("Scene Tree")
{

}

UI_SceneTree::UI_SceneTree(const char* layerName) : UILayer(layerName)
{

}

void UI_SceneTree::renderElement(BaseObject* object)
{
	unsigned childCount = object->getChildrenCount();
	bool isLeaf = !childCount;

	ImGuiTreeNodeFlags flags = ((Scene::activeScene->getSelectedObject() == object) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow; //Make better way to compare objects instead of their raw pointers

	if (isLeaf)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	//Leaf nodes always return true for isOpen
	bool isOpen = ImGui::TreeNodeEx((void*)object, flags, object->getName().c_str());

	//Igonre click events on the root
	if (object->getParent() != nullptr)
	{
		if (ImGui::IsItemClicked())
		{
			Scene::activeScene->setSelectedObject(object);
			std::cout << "Selected: " << object->getName() << std::endl;
		}

		else if (ImGui::IsItemClicked(1))
		{
				object->getParent()->removeChild(object);
				std::cout << "Removed object: " << object->getName() << std::endl;
		}

		if (ImGui::BeginDragDropSource())
		{
				ImGui::SetDragDropPayload("_TREENODE", &object, sizeof(object));
				ImGui::Text(object->getName().c_str());
				ImGui::EndDragDropSource();
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_TREENODE"))
		{
			BaseObject** droppedObject = (BaseObject**)payload->Data;
			std::cout << "Dropped: " << (*droppedObject)->getName() << " On: " << object->getName() << std::endl;
			//attach the dragged object to the target
			(*droppedObject)->attachTo(object);
		}
		ImGui::EndDragDropTarget();
	}

	if (isOpen)
	{
		for (size_t i = 0; i < object->getChildrenCount(); i++)
		{
			BaseObject* child = object->children[i];
			if (child != nullptr)
			{
				renderElement(child);
			}
		}

		ImGui::TreePop();
	}
}

void UI_SceneTree::renderLayer()
{
	ImGui::Begin(layerName.c_str());

	BaseObject* root = &Scene::activeScene->root;
	//dont create a node but pass the parameters to the renderElement function
	renderElement(root);

	ImGui::End();

}

UI_SceneTree* UI_SceneTree::clone()
{
	return new UI_SceneTree(*this);
}