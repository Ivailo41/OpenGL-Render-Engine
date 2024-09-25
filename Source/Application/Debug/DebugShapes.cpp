#include "DebugShapes.h"

void DebugShapes::drawLine(Point start, Point end, Color color)
{
	getContainerWithColor(color).pushLine(Line(start, end));
}

void DebugShapes::drawBox(Point topLeftBack, Point bottomRightFront, Color color)
{
	//get all vertices of the box
	Point topRightBack(bottomRightFront.x, topLeftBack.y, topLeftBack.z);
	Point topLeftFront(topLeftBack.x, topLeftBack.y, bottomRightFront.z);
	Point topRightFront(bottomRightFront.x, topLeftBack.y, bottomRightFront.z);

	Point bottomLeftBack(topLeftBack.x, bottomRightFront.y, topLeftBack.z);
	Point bottomRightBack(bottomRightFront.x, bottomRightFront.y, topLeftBack.z);
	Point bottomLeftFront(topLeftBack.x, bottomRightFront.y, bottomRightFront.z);

	LinesContainer& linesContainer = getContainerWithColor(color);

	//Top square
	linesContainer.pushLine(Line(topLeftBack, topRightBack));
	linesContainer.pushLine(Line(topRightBack, topRightFront));
	linesContainer.pushLine(Line(topRightFront, topLeftFront));
	linesContainer.pushLine(Line(topLeftFront, topLeftBack));

	//vertical lines
	linesContainer.pushLine(Line(topLeftBack, bottomLeftBack));
	linesContainer.pushLine(Line(topRightBack, bottomRightBack));
	linesContainer.pushLine(Line(topRightFront, bottomRightFront));
	linesContainer.pushLine(Line(topLeftFront, bottomLeftFront));

	//bottom square
	linesContainer.pushLine(Line(bottomLeftBack, bottomRightBack));
	linesContainer.pushLine(Line(bottomRightBack, bottomRightFront));
	linesContainer.pushLine(Line(bottomRightFront, bottomLeftFront));
	linesContainer.pushLine(Line(bottomLeftFront, bottomLeftBack));

}

void DebugShapes::drawDebugShapes(Camera* camera)
{
	//bind shader
	debugShader->use();
	
	camera->updateCamera();

	unsigned size = containers.size();
	for (size_t i = 0; i < size; i++)
	{
		containers[i].drawLines(glm::mat4(1));
	}
}

DebugShapes::DebugShapes()
{
	debugShader = &Shader::shaders[3]; //get the debug shader from the public shader list
}

LinesContainer& DebugShapes::getContainerWithColor(const Color& color)
{
	unsigned size = containers.size();
	for (size_t i = 0; i < size; i++)
	{
		if(containers[i].getColor() == color)
		{
			return containers[i];
		}
	}

	containers.push_back(LinesContainer(color));
	return containers[size];
}
