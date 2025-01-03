#include "config.hpp"

int main(int argc, char **argv) {
	(void)argc; (void)argv;

	try {
		// Create a window
		WindowsHandler windowsHandler;
		windowsHandler.createWindow(200, 200, 800, 600, "Window");

		// Main loop
		while (!glfwWindowShouldClose(*windowsHandler.front())) {
			// // Rendering
			// Rendering(*windowsHandler[windowIndex]);

			// Swap buffers
			glfwSwapBuffers(*windowsHandler.front());

			// Poll events
			glfwPollEvents();
		}
	}
	catch(const exception& e) {
		cerr << BRed <<  "Critical Error : " << e.what() << ResetColor <<'\n';
		exit(EXIT_FAILURE);
	}
}
