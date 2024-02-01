#pragma once
#include <string>
#include "../../Includes/imGuiInclude.h"

class UIElement
{
public:
	UIElement();
	UIElement(const std::string& name);
	UIElement(const UIElement& other) = delete;
	virtual ~UIElement();

	virtual void renderElement() = 0;

	void setName(const std::string& name);
	std::string getName() const { return name;}

protected:
	std::string name;

};

