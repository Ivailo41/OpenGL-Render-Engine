#pragma once

#include <vector>
#include <GL/glew.h>
#include "../Primitives.h"
#include "../Shader.h"
#include "../glm/gtc/type_ptr.hpp"

class LinesContainer
{
public:
	void pushLine(const Line& line);
	void drawLines(glm::mat4 transform) const;

	Color getColor() const { return lineColor; }

	bool isEmpty() const { return lines.empty(); }

public:
	//Copying may cause problems with buffers so think on adding copy ctor and =operator later
	LinesContainer();
	LinesContainer(const Color&);
	~LinesContainer();

private:
	std::vector<Line> lines;
	Color lineColor;
	GLuint VBO;
};