#include "config.hpp"

void renderLoop() {
	// Rendering code goes here
}

int main() {

	Logger& logger = Logger::getInstance();

	logger.enableConsoleOutput(true);
	logger.setLogLevel(Logger::LogLevel::TRACE);
	logger.info("Game Engine started");

	GE::Window window(100, 100, 800, 600, "Game Engine Window", 4.2f, &logger);

	window.mainLoop(renderLoop);
	logger.info("Game Engine stopped");

	return 0;
}