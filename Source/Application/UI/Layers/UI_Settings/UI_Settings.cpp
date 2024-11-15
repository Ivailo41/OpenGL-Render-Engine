#include "UI_Settings.h"

void UI_Settings::renderLayer()
{
	ImGui::Begin(layerName.c_str());
	ImGui::Checkbox("Bloom", &useBloom);

	if (ImGui::SliderFloat("Gamma", &gamma, 0.0f, 5.0f))
	{
		Shader::activeShader->setFloat("gamma", gamma);
		//find way to access gamma and exposure in the FrameQuad drawFrameQuad function
	}

	if (ImGui::SliderFloat("Exposure", &exposure, 0.0f, 5.0f))
	{
		Shader::activeShader->setFloat("exposure", exposure);
	}

	if(useBloom)
	{
		if(ImGui::SliderFloat("Threshold", &threshold, 0.0f, 1.0f))
		{
			
		}
		if(ImGui::InputInt("Steps", &steps))
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

	if (ImGui::CollapsingHeader("Textures"))
	{
		std::vector<Texture*>& textures = Scene::activeScene->textures;
		unsigned matCount = textures.size();

		for (size_t i = 0; i < matCount; i++)
		{
			if (ImGui::Selectable(textures[i]->path.c_str()))
			{

			}
		}
	}

	ImGui::End();
}

UI_Settings* UI_Settings::clone()
{
	return new UI_Settings(*this);
}

UI_Settings::UI_Settings() : UILayer("Settings"), steps(10), threshold(0.8), useBloom(false), gamma(1), exposure(0.5)
{

}
