#pragma once
#include "../UILayer.h"

class UI_Settings : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_Settings* clone() override;

	bool getBloom() const { return useBloom; }
	float getTreshold() const { return threshold; }
	int getSteps() const { return steps; }

public:
	UI_Settings();

private:
	bool useBloom;
	float threshold;
	int steps;
};