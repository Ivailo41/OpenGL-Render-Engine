#pragma once
#include "UILayer.h"
#include <glm.hpp>
#include "../../Renderer/FrameBuffer.h"
#include "../../Renderer/Renderer.h"
#include "../../Core/Window.h"

class UI_Scene : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_Scene* clone() override;

	bool isViewMode() const { return viewMode; }
	ImVec2 getWindowSpace() const { return windowSpace; }

public:
	UI_Scene(Window* window, Renderer* renderer);
	~UI_Scene() override;

private:
	Window* window;
	ImVec2 windowSpace;

	bool viewMode;

	Renderer* renderer;

	//gizmo temp variable
	ImGuizmo::OPERATION operation;
};