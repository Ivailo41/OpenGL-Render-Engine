#pragma once
#include "../UILayer.h"
#include "../../../glm/glm.hpp"
#include "../../../GL_Objects/FrameBuffer.h"
#include "../../../GL_Objects/Window.h"

class UI_Scene : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_Scene* clone() override;

	const FrameBuffer& getFrameBuffer() const { return fbuffer; }

	unsigned getTexture() const { return fbuffer[0]; }
	unsigned getLightTexture() const { return fbuffer[1]; }

public:
	UI_Scene(Window* window);
	~UI_Scene() override;

private:
	Window* window;
	unsigned colorBuffers[2];
	unsigned fbo;
	ImVec2 windowSpace;

	FrameBuffer fbuffer;
};