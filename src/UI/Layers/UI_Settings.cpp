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

	//TEST to see the loaded materials and textures
	if(ImGui::CollapsingHeader("Materials"))
	{
		auto& materials = resourceManager->getMaterials();

		for (auto& material : materials)
		{
			if(ImGui::CollapsingHeader(material.first.c_str()))
			{
				if(ImGui::CollapsingHeader("Diffuse texture"))
				{
					auto& textures = resourceManager->getTextures();

					for (auto& texture : textures)
					{
						if (ImGui::Selectable(texture.first.c_str()))
						{
							material.second.setTexture(&texture.second, 0);
						}
					}
				}

				if (ImGui::CollapsingHeader("ORM texture"))
				{
					auto& textures = resourceManager->getTextures();

					for (auto& texture : textures)
					{
						if (ImGui::Selectable(texture.first.c_str()))
						{
							material.second.setTexture(&texture.second, 1);
						}
					}
				}

				if (ImGui::CollapsingHeader("Normal texture"))
				{
					auto& textures = resourceManager->getTextures();

					for (auto& texture : textures)
					{
						if (ImGui::Selectable(texture.first.c_str()))
						{
							material.second.setTexture(&texture.second, 2);
						}
					}
				}
			}
		}
	}

	ImGui::End();
}

UI_Settings* UI_Settings::clone()
{
	return new UI_Settings(*this);
}

UI_Settings::UI_Settings(Renderer* renderer, ResourceManager* resourceManager) : UILayer("Settings"), renderer(renderer), resourceManager(resourceManager)
{

}
