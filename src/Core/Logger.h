#pragma once

#include <ctime>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>
#include <mutex>
#include <string>
#include <vector>

enum class LogLevel
{
	TRACE,
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	FATAL
};

class LogSink
{
public:
	virtual ~LogSink() = default;
	virtual void onLog(const std::string& message, LogLevel level, int line) = 0;

protected:
	std::string getCurrentTime() const;
	std::string levelToString(LogLevel level) const;
};

class Logger
{
public:
	static Logger& getInstance();

	void log(const std::string& message, LogLevel level, int line);
	void addSink(std::shared_ptr<LogSink> sink);

private:
	Logger() = default;

private:
	std::mutex logMutex;
	std::ofstream logFile;

	std::vector<std::shared_ptr<LogSink>> sinks;
};

class ConsoleLogSink : public LogSink
{
public:
	virtual void onLog(const std::string& message, LogLevel level, int line) override;
};

#define LOG(msg, lvl) Logger::getInstance().log(msg, lvl, 0)
#define LOG_TRACE(msg) Logger::getInstance().log(msg, LogLevel::TRACE, 0)
#define LOG_DEBUG(msg) Logger::getInstance().log(msg, LogLevel::DEBUG, 0)
#define LOG_INFO(msg) Logger::getInstance().log(msg, LogLevel::INFO, 0)
#define LOG_WARNING(msg) Logger::getInstance().log(msg, LogLevel::WARNING, 0)
#define LOG_ERROR(msg) Logger::getInstance().log(msg, LogLevel::ERROR, 0)
#define LOG_FATAL(msg) Logger::getInstance().log(msg, LogLevel::FATAL, 0)