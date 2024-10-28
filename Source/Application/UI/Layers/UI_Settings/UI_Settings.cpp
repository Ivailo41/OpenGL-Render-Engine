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

	ImGui::End();
}

UI_Settings* UI_Settings::clone()
{
	return new UI_Settings(*this);
}

UI_Settings::UI_Settings() : UILayer("Settings"), steps(10), threshold(0.8), useBloom(false), gamma(1), exposure(0.5)
{

}
