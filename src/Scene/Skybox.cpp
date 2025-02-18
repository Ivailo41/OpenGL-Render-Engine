#include "Skybox.h"

void Skybox::render(const Camera* camera) const
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    skyboxShader->use();

    glm::mat4 view = glm::mat4(glm::mat3(camera->getViewMat()));
    skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("projection", camera->getPerspectiveMat());

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    cubemap->bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

Skybox::Skybox(const Cubemap* cubemap, const Shader* skyboxShader) : cubemap(cubemap), skyboxShader(skyboxShader)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PRIMITIVE_SHAPES::cubeVertices), PRIMITIVE_SHAPES::cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

Skybox::~Skybox()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
