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
		std::vector<Material*>& materials = Scene::activeScene->materials;
		unsigned matCount = materials.size();

		for (size_t i = 0; i < matCount; i++)
		{
			if(ImGui::CollapsingHeader(materials[i]->getName().c_str()))
			{
				if(ImGui::CollapsingHeader("Diffuse texture"))
				{
					std::vector<Texture*>& textures = Scene::activeScene->textures;
					unsigned matCount = textures.size();

					for (size_t j = 0; j < matCount; j++)
					{
						if (ImGui::Selectable(textures[j]->path.c_str()))
						{
							materials[i]->setTexture(textures[j], 0);
						}
					}
					//ImGui::BeginCombo("test", "");

				}

				if (ImGui::CollapsingHeader("ORM texture"))
				{
					std::vector<Texture*>& textures = Scene::activeScene->textures;
					unsigned matCount = textures.size();

					for (size_t j = 0; j < matCount; j++)
					{
						if (ImGui::Selectable(textures[j]->path.c_str()))
						{
							materials[i]->setTexture(textures[j], 1);
						}
					}
				}

				if (ImGui::CollapsingHeader("Normal texture"))
				{
					std::vector<Texture*>& textures = Scene::activeScene->textures;
					unsigned matCount = textures.size();

					for (size_t j = 0; j < matCount; j++)
					{
						if (ImGui::Selectable(textures[j]->path.c_str()))
						{
							materials[i]->setTexture(textures[j], 2);
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

UI_Settings::UI_Settings(FileManager* fileman, Renderer* renderer) : UILayer("Settings"), fileman(fileman), renderer(renderer)
{

}
