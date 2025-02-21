#pragma once
#include "UILayer.h"
#include <glm.hpp>
#include "../../Renderer/FrameBuffer.h"
#include "../../Core/Window.h"

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

	bool viewMode;

	FrameBuffer fbuffer;

	//gizmo temp variable
	ImGuizmo::OPERATION operation;
};