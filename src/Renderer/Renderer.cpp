#include "Renderer.h"

Renderer::Renderer() : colorFrameBuffer(2, true), screneFrameBuffer(2, false)
{

}

bool Renderer::init(Window* window)
{
    if(running)
    {
        return true;
    }

	LOG_TRACE("Renderer initialized!");

	pbrShader = Shader::findShader("PBRShader");
	shadowShader = Shader::findShader("ShadowShader");
	debugShader = Shader::findShader("DebugShader");
	tangentShader = Shader::findShader("TangentShader");
	frameQuadShader = Shader::findShader("FramequadShader");
	skyboxShader = Shader::findShader("SkyboxShader");

	if (!pbrShader || !shadowShader || !debugShader || !tangentShader || !frameQuadShader || !skyboxShader)
    {
		LOG_ERROR("One or more shaders could not be found!");
		return false;
	}

	colorFrameBuffer.genFrameBuffer(window->getWidth(), window->getHeight());
	screneFrameBuffer.genFrameBuffer(window->getWidth(), window->getHeight());
    shadowFrameBuffer.genFrameBuffer(Light::SHADOW_WIDTH, Light::SHADOW_HEIGHT);

    FrameQuad::initFrameQuad();

    running = true;
    return true;
}

void Renderer::stop()
{
	// Cleanup resources if needed
    running = false;
	LOG_TRACE("Renderer stopped!");
}

void Renderer::onWindowResize(int width, int height)
{
	// Resize the frame buffers to match the new window size
	colorFrameBuffer.genFrameBuffer(width, height);
	screneFrameBuffer.genFrameBuffer(width, height);

	// Update the viewport to match the new window size
	glViewport(0, 0, width, height);
}

void Renderer::renderScene(Scene* scene, Window* window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //SHADOW PASS START
    glViewport(0, 0, Light::SHADOW_WIDTH, Light::SHADOW_HEIGHT); //set the viewport to the shadow map size
    shadowFrameBuffer.bind();
    glClear(GL_DEPTH_BUFFER_BIT); //clear the depth buffer for the shadow map

    //use depth shader and pass the light position to it
    shadowShader->use();

    for (unsigned i = 0; i < scene->lights.size(); i++)
    {
        //attach each cubemap to the framebuffer
		if (scene->lights[i]->getLightType() != LightType::POINT)
			continue; //skip non-point lights for shadow mapping
        shadowFrameBuffer.attachCubemap(dynamic_cast<PointLight*>(scene->lights[i])->getShadowCubemap());
        glClear(GL_DEPTH_BUFFER_BIT); //clear the depth buffer for the shadow map
        shadowShader->use();
        scene->lights[i]->sendShadowDataToShader(*shadowShader, i);

        for (SceneNode* child : scene->sceneObjects) {
			child->draw(shadowShader); //draws object to the shadow map
        }
        //scene->root.draw(shadowShader); //draws object to the shadow map
        //draw scene from each light perspective
    }
    shadowFrameBuffer.unbind(); //unbind the shadow framebuffer
    //SHADOW PASS END

    //START OF SCENE OBJECTS RENDERING
    window->resetViewport();
    colorFrameBuffer.bind();

    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_FRONT);

    //The PBR shader
    pbrShader->use();
    pbrShader->setFloat("threshold", bloomThreshold);
    pbrShader->setFloat("gamma", gamma);
	pbrShader->setFloat("exposure", exposure);

    //PASS LIGHTS TO SHADER         //LIGHTS ARE UPDATED EVERY FRAME, MAKE IT ONLY WHEN THEY ARE MOVED
    for (unsigned i = 0; i < scene->lights.size(); i++)
    {
        //!!!MAXIMUM OF 16 TEXTURES CAN BE BOUND AT ONCE, MAKE SURE TO NOT EXCEED THE LIMIT, LATER ON FIND BETTER APPROACH!!!
        if (scene->lights[i]->getLightType() != LightType::POINT)
            continue; //skip non-point lights for shadow mapping
		PointLight* pointLight = dynamic_cast<PointLight*>(scene->lights[i]);

        glActiveTexture(GL_TEXTURE0 + 3 + i);
        glBindTexture(GL_TEXTURE_CUBE_MAP, pointLight->getShadowCubemap());
        scene->lights[i]->sendToShader(*pbrShader, i);
        pbrShader->setFloat(std::string("far_plane[" + std::to_string(i) + "]").c_str(), pointLight->getShadowFar());
        pbrShader->setInt(std::string("depthMap[" + std::to_string(i) + "]").c_str(), 3 + i); //bind the shadow map texture to the shader
    }

    scene->getActiveCamera()->updateCamera();
    for (SceneNode* child : scene->sceneObjects) {
        child->draw(pbrShader); //draws object
    }
    //scene->root.draw(); //draws object

    glBindVertexArray(0); //unbind the last vertex array object which belongs to the last rendered mesh
    //sinse debug doesnt use VAO and doesnt bind one

    //IF WE DONT STOP THE DRAWING TO THE ATTACHMENT1 THE OTHER SHADERS WILL DRAW TO THE BLOOM TEXTURE TOO
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    //END OF SCENE OBJECTS RENDERING - PUT THAT IN THE FUNCTION

	debugShader->use();
	scene->getActiveCamera()->updateCamera();
    debugShapes.drawDebugShapes();

    for (SceneNode* child : scene->sceneObjects) {
        child->drawDebug();
    }
    //scene->root.drawDebug();

    //visualise tangents, normals and bitangents
    if(drawTangents)
    {
        tangentShader->use();
        tangentShader->setFloat("lineWidth", tangentLength); //set the line width for the tangent shader
        scene->getActiveCamera()->updateCamera();
        glLineWidth(3.0f);
        for (SceneNode* child : scene->sceneObjects) {
            child->draw(tangentShader, GL_POINTS); //draws object with tangent shader, used for debugging tangents
        }
        //scene->root.draw(tangentShader, GL_POINTS); //draws object with tangent shader, used for debugging tangents
    }

    //make skybox member of scene
    skyboxShader->use();
    glm::mat4 view = glm::mat4(glm::mat3(scene->getActiveCamera()->getViewMat()));
    skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("projection", scene->getActiveCamera()->getPerspectiveMat());
    scene->activeSkybox->render();

    //apply bloom effect, currently the bloom is performance heavy, search for another approach
    if (isBloomEnabled)
    {
        /*resultTexture = bloomPP.applyEffect(firstPassBuffer, mainUI.getSceneLayer().getFrameBuffer());
        bloomPP.setSteps(settingsLayer.getSteps());*/
    }
    else
    {
        sceneTexture = colorFrameBuffer[0];
    }

    //draw the final result to the screne frame buufer, TODO: change the way of gettting gamma and exposure
	screneFrameBuffer.bind();
	frameQuadShader->use();
    frameQuadShader->setInt("scene", 0);
    frameQuadShader->setFloat("exposure", exposure);
    frameQuadShader->setFloat("gamma", gamma);
    FrameQuad::drawFrameQuad(sceneTexture);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}