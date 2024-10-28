#pragma once
#include "../UILayer.h"
#include "../../../Shader.h"

class UI_Settings : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_Settings* clone() override;

	bool isUsingBloom() const { return useBloom; }
	float getTreshold() const { return threshold; }
	int getSteps() const { return steps; }

	float getGamma() const { return gamma; }
	float getExposure() const { return exposure; }

public:
	UI_Settings();

private:
	bool useBloom;
	float threshold;
	int steps;

	float gamma;
	float exposure;
};