#pragma once
#include "UIElement.h"
#include "../../Scene/BaseObject.h"
class UISceneNode : public UIElement
{
public:
	UISceneNode(const std::string& name, BaseObject* object);
	UISceneNode(const UISceneNode& other) = delete;
	virtual ~UISceneNode();

	virtual void renderElement() override;

protected:
	BaseObject* object;

};