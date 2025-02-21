#include "UISceneTree.h"

UI_SceneTree::UI_SceneTree() : UILayer("Scene Tree")
{

}

UI_SceneTree::UI_SceneTree(const char* layerName) : UILayer(layerName)
{

}

void UI_SceneTree::renderElement(BaseObject* object, const ImGuiTextFilter& filter)
{
	//If the object doesnt pass the filter, search for its children
	if(!filter.PassFilter(object->getName().c_str()))
	{
		for (size_t i = 0; i < object->getChildrenCount(); i++)
		{
			BaseObject* child = object->children[i];
			if (child != nullptr)
			{
				renderElement(child, filter);
			}
		}
		return;
	}

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
			ImGui::OpenPopup("my_select_popup");
		}

		if (ImGui::BeginPopup("my_select_popup"))
		{
			ImGui::SeparatorText(object->getName().c_str());
			if(ImGui::MenuItem("Copy"))
			{

			}
			if (ImGui::MenuItem("Paste"))
			{

			}
			if (ImGui::MenuItem("Select"))
			{
				Scene::activeScene->setSelectedObject(object);
				std::cout << "Selected: " << object->getName() << std::endl;
			}
			if(ImGui::MenuItem("Delete"))
			{
				object->getParent()->removeChild(object);
				std::cout << "Removed object: " << object->getName() << std::endl;
			}
			ImGui::EndPopup();
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
			//Prevent attaching a parent to its children
			if(!object->isChildOf(*droppedObject))
			{
				std::cout << "Dropped: " << (*droppedObject)->getName() << " On: " << object->getName() << std::endl;
				//attach the dragged object to the target
				(*droppedObject)->attachTo(object);
			}
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
				renderElement(child, filter);
			}
		}

		ImGui::TreePop();
	}
}

void UI_SceneTree::renderLayer()
{
	ImGui::Begin(layerName.c_str());

	static ImGuiTextFilter filter;
	filter.Draw("Seach");

	BaseObject* root = &Scene::activeScene->root;
	//dont create a node but pass the parameters to the renderElement function
	renderElement(root, filter);

	ImGui::End();

}

UI_SceneTree* UI_SceneTree::clone()
{
	return new UI_SceneTree(*this);
}