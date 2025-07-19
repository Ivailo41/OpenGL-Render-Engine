#include "UI_Console.h"

void UI_Console::renderLayer()
{
	ImGui::Begin(layerName.c_str());
	for (auto& message : logMessages)
	{
		ImGui::Text("%s", message.c_str());
	}
	ImGui::End();
}

UI_Console* UI_Console::clone()
{
	return new UI_Console(layerName.c_str());
}

void UI_Console::addLogMessage(const std::string& message)
{
	logMessages.push_back(message);
}

UI_Console::UI_Console()
{
	layerName = "Console";
}

UI_Console::UI_Console(const char* name) : UILayer(name)
{
	//nothing to do here
}

void UILogSink::onLog(const std::string& message, LogLevel level, int line)
{
	std::stringstream ss;
	ss << "[" << getCurrentTime() << "] [" << levelToString(level) << "] " << message;

	if(consoleLayer)
	{
		consoleLayer->addLogMessage(ss.str());
	}
}

UILogSink::UILogSink(UI_Console* consoleLayer) : consoleLayer(consoleLayer)
{
	// nothing to do here
}
