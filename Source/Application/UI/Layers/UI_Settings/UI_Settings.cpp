#include "UI_Settings.h"

void UI_Settings::renderLayer()
{
	ImGui::Begin(layerName.c_str());
	ImGui::Checkbox("Bloom", &useBloom);

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

UI_Settings::UI_Settings() : UILayer("Settings"), steps(10), threshold(0.8), useBloom(false)
{

}
