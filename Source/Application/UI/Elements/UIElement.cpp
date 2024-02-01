#include "UIElement.h"

UIElement::UIElement()
{
}

UIElement::UIElement(const std::string& name)
{
	this->name = name;
}

UIElement::~UIElement()
{
}

void UIElement::setName(const std::string& name)
{
	this->name = name;
}
