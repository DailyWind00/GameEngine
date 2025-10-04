#include <Logger.hpp>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace GE::Core {

	#pragma region Public Methods

	void Logger::enableFileOutput(const std::string& filename){
		std::lock_guard<std::mutex> lock(logMutex);
		
		fileStream.open(filename, std::ios::app);
		toFile = fileStream.is_open();
	}

    void Logger::enableConsoleOutput(bool enable) {
		std::lock_guard<std::mutex> lock(logMutex);

		toConsole = enable;
	}

	void Logger::log(LogLevel level, const std::string& message) {
		std::lock_guard<std::mutex> lock(logMutex);

		if (level < currentLogLevel)
			return;

		size_t threadID = getThreadIndex();

		std::string levelStr = levelToString(level);
		std::ostringstream paddedLevel;
		paddedLevel << "[" << levelStr << "]" << std::string(8 - levelStr.length(), ' ');

		std::ostringstream oss;
		oss << "[T" << std::setw(2) << threadID << "] "
			<< "[" << getTimestamp() << "] "
			<< paddedLevel.str()
			<< message << std::endl;

		std::string output = oss.str();

		if (toConsole)
			std::cout << output;

		if (toFile && fileStream.is_open())
			fileStream << output;
	}

	void Logger::setLogLevel(LogLevel level) {
		std::lock_guard<std::mutex> lock(logMutex);
		currentLogLevel = level;
	}

	void	Logger::trace(const std::string& message)	{ log(LogLevel::TRACE, message);   }
	void	Logger::debug(const std::string& message)	{ log(LogLevel::DEBUG, message);   }
	void	Logger::info(const std::string& message)	{ log(LogLevel::INFO, message);    }
	void	Logger::warn(const std::string& message)	{ log(LogLevel::WARNING, message); }
	void	Logger::error(const std::string& message)	{ log(LogLevel::ERROR, message);   }
	void	Logger::fatal(const std::string& message)	{ log(LogLevel::FATAL, message);   }

	#pragma endregion

	#pragma region Private Methods

	std::string Logger::getTimestamp() {

		auto now = std::time(nullptr);

		std::ostringstream ss;
		ss << std::put_time(std::localtime(&now), "%H:%M:%S");

		return ss.str();
	}

	std::string Logger::levelToString(LogLevel level) {
		switch (level) {
			case LogLevel::TRACE:   return "TRACE";
			case LogLevel::DEBUG:   return "DEBUG";
			case LogLevel::INFO:    return "INFO";
			case LogLevel::WARNING: return "WARNING";
			case LogLevel::ERROR:   return "ERROR";
			case LogLevel::FATAL:   return "FATAL";
			default:                return "UNKNOWN";
		}
	}

	int Logger::getThreadIndex() {
		static int nextThreadId = 0;

		std::lock_guard<std::mutex> lock(mapMutex);

		auto id = std::this_thread::get_id();
		auto it = threadMap.find(id);

		if (it != threadMap.end())
			return it->second;
		else
			return threadMap[id] = nextThreadId++;
	}

	#pragma endregion

} // namespace GE::Core