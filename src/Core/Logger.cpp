#include "Logger.h"

std::string LogSink::getCurrentTime() const
{
	auto now = std::chrono::system_clock::now();
	std::time_t timeNow = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&timeNow), "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

std::string LogSink::levelToString(LogLevel level) const
{
	switch (level)
	{
	case LogLevel::TRACE: return std::string("TRACE");
		break;
	case LogLevel::DEBUG:  return std::string("\033[1;32mDEBUG\033[0m");
		break;
	case LogLevel::INFO:  return std::string("\033[1;34mINFO\033[0m");
		break;
	case LogLevel::WARNING:  return std::string("\033[1;33mWARNING\033[0m");
		break;
	case LogLevel::ERROR:  return std::string("\033[1;31mERROR\033[0m");
		break;
	case LogLevel::FATAL:  return std::string("\033[1;37mFATAL\033[0m");
		break;
	default:  return std::string("UNKNOWN");
		break;
	}
}

Logger& Logger::getInstance()
{
	static Logger logger;
	return logger;
}

void Logger::log(const std::string& message, LogLevel level, int line)
{
	std::lock_guard<std::mutex> lock(logMutex);

	for(auto& sink : sinks)
	{
		sink->onLog(message, level, line);
	}
}

void Logger::addSink(std::shared_ptr<LogSink> sink)
{
	std::lock_guard<std::mutex> lock(logMutex);
	sinks.push_back(sink);
}

void ConsoleLogSink::onLog(const std::string& message, LogLevel level, int line)
{
	std::cout << '[' << getCurrentTime() << "][" << levelToString(level) << "] : " << message << std::endl;
}