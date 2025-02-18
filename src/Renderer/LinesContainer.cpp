#include "LinesContainer.h"

void LinesContainer::pushLine(const Line& line)
{
	lines.push_back(line);
}

void LinesContainer::drawLines(glm::mat4 transform) const
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(Line), &lines[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Enable position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	unsigned int colorLoc = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "color");
	glUniform3f(colorLoc, lineColor.r, lineColor.g, lineColor.b);

	unsigned int transformLoc = glGetUniformLocation(Shader::activeShader->getShaderProgram(), "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glDrawArrays(GL_LINES, 0, lines.size() * 2);
}

LinesContainer::LinesContainer() : lineColor(1, 0, 0) //default line color is red
{
	glGenBuffers(1, &VBO);
}

LinesContainer::LinesContainer(const Color& color) : lineColor(color)
{
	glGenBuffers(1, &VBO);
}

LinesContainer::~LinesContainer()
{
	//glDeleteBuffers(1, &VBO); //CAUSES THE POST PROCCESS QUAD TO RENDER INCORRECTLY
}