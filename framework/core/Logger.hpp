#pragma once

// Includes
# include "Singleton.hpp"

// System includes
# include <fstream>
# include <mutex>
# include <thread>
# include <unordered_map>

namespace GE::Core {
	/**
	 * @brief The Logger class provides a simple logging mechanism for the application.
	 * 
	 * The logger can output messages to both the console and a file, and it supports different log levels:
	 * TRACE, DEBUG, INFO, WARNING, ERROR, and FATAL.
	 * 
	 * @note This logger is thread-safe and can log messages to both console and file.
	 * Consider not using it in performance-critical sections of the code, as logging can introduce some overhead.
	 */
	class Logger : public Singleton<Logger> {
		friend class Singleton<Logger>;
		
		public:
			// Log levels
			enum class LogLevel {
				TRACE,
				DEBUG,
				INFO,
				WARNING,
				ERROR,
				FATAL
			};

			void enableFileOutput(const std::string& filename);
    		void enableConsoleOutput(bool enable);

			void log(LogLevel level, const std::string& message);
			void setLogLevel(LogLevel level);

			// Convenience methods for different log levels

			void trace(const std::string& message);
			void debug(const std::string& message);
			void info(const std::string& message);
			void warn(const std::string& message);
			void error(const std::string& message);
			void fatal(const std::string& message);

		private:
		    std::string getTimestamp();
			std::string levelToString(LogLevel level);
			int getThreadIndex();

			LogLevel currentLogLevel = LogLevel::INFO;

			bool toConsole = true;
			bool toFile = false;
			std::ofstream fileStream;

			std::unordered_map<std::thread::id, int> threadMap;

			std::mutex mapMutex;
			std::mutex logMutex;
	};
} // namespace GE::Core