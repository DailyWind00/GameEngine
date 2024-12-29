#include "config.hpp"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

// Initialize GLFW and create a window
GLFWwindow	*CreateWindow() {
	GLFWwindow	*window;

	if (!glfwInit())
		throw runtime_error("Failed to initialize GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL 4.5
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac-os compatibility
	glfwWindowHint(GLFW_SAMPLES, 4);

	printVerbose("GLFW initialized");

	const string title = "GameEngine tests by DailyWind";
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title.c_str(), nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		throw runtime_error("Failed to create window");
	}
	printVerbose("Window \"" + title + "\" created");

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		throw runtime_error("Failed to initialize GLAD");
	}
	printVerbose("GLAD initialized");

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	return window;
}

// Destroy the window and terminate GLFW
// Should only be called if CreateWindow was successful
void	DestroyWindow(GLFWwindow *window) {
	if (!window)
		return;
	glfwDestroyWindow(window);
	glfwTerminate();
	printVerbose("Window destroyed");
}

