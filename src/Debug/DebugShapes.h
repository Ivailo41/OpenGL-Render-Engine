#pragma once
#include "LinesContainer.h"
#include "../Camera.h"

class DebugShapes
{
public:
	void drawLine(Point start, Point end, Color color);				//will add line to a container with this color
	void drawBox(Point topLeft, Point bottomRight, Color color);	//will add lines that form a box to a container with this color

	void drawDebugShapes(Camera* camera);							//will draw all the lines from each container

public:
	DebugShapes();

private:
	LinesContainer& getContainerWithColor(const Color& color);

private:
	std::vector<LinesContainer> containers;
	Shader* debugShader;
};