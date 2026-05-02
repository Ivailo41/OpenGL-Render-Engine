#pragma once
#include "UILayer.h"
#include "../../Core/Logger.h"

class UI_Console : public UILayer
{
public:
	virtual void renderLayer() override;
	virtual UI_Console* clone() override;

	void addLogMessage(const std::string& message);

public:
	UI_Console();
	UI_Console(const char* name);

private:
	std::vector<std::string> logMessages; // Store log messages to render them later
};

class UILogSink : public LogSink
{
public:
	virtual void onLog(const std::string& message, LogLevel level, int line) override;
	virtual ~UILogSink() = default;

	UILogSink(UI_Console* consoleLayer);

protected:
	UI_Console* consoleLayer = nullptr;
};