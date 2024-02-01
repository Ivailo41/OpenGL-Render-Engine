#include "UILayer.h"

UILayer::UILayer()
{
	layerName = "New Layer";
}

UILayer::UILayer(const char* name)
{
	layerName = name;
}

UILayer::~UILayer()
{
}

void UILayer::setLayerName(const char* name)
{
	layerName = name;
}
