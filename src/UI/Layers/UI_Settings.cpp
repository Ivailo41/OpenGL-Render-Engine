#include "UI_Settings.h"

void UI_Settings::renderLayer()
{
	ImGui::Begin(layerName.c_str());

	ImGui::SliderFloat("Gamma", &renderer->gamma, 0.0f, 5.0f);

	ImGui::SliderFloat("Exposure", &renderer->exposure, 0.0f, 5.0f);

	if (ImGui::Checkbox("Bloom", &renderer->isBloomEnabled));
	if(renderer->isBloomEnabled)
	{
		if(ImGui::SliderFloat("Threshold", &renderer->bloomThreshold, 0.0f, 1.0f))
		{
			
		}
		if(ImGui::InputInt("Steps", &renderer->bloomSteps))
		{

		}
	}
	if (ImGui::Checkbox("Draw Tangents", &renderer->drawTangents));
	if (renderer->drawTangents)
	{
		if (ImGui::SliderFloat("Tangent Length", &renderer->tangentLength, 0.0f, 1.0f))
		{

		}
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}

UI_Settings* UI_Settings::clone()
{
	return new UI_Settings(*this);
}

UI_Settings::UI_Settings(Renderer* renderer, ResourceManager* resourceManager) : UILayer("Settings"), renderer(renderer), resourceManager(resourceManager)
{

}
