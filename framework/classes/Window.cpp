#include "Window.hpp"

namespace GE {

	# pragma region Constructors & Destructors

	Window::Window(int posX, int posY, int width, int height, const std::string &title, const float &GLversion, Core::Logger *logger) : logger(logger) {
		if (logger) logger->trace("Creating window");

		if (!glfwInit())
			throw std::runtime_error("Failed to initialize GLFW");

		if (logger) logger->trace("GLFW initialized");

		// TODO: make the OpenGL context in a separate class, need to have an integrated framework before that
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, static_cast<int>(GLversion));
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, static_cast<int>(GLversion * 10) % 10);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac-os compatibility
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (!window)
			throw std::runtime_error("Failed to create window");
		
		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			glfwDestroyWindow(window);
			throw std::runtime_error("Failed to initialize GLAD");
		}
		if (logger) logger->trace("GLAD initialized");

		glfwSetWindowPos(window, posX, posY);

		if (logger) logger->info("Window created");
	}

	Window::~Window() {
		glfwDestroyWindow(window);
		glfwTerminate();

		if (logger) logger->info("Window destroyed");
	}

	# pragma endregion

	# pragma region Private functions

	// Update the frame rate and the fps counters
	void	Window::updateFrameRate() {
		static double	lastTime = glfwGetTime();
		static double	lastFrame = glfwGetTime();
		static size_t	frames = 0;

		frames++;

		// Update the frame time
		double currentTime = glfwGetTime();
		this->frameTime = (currentTime - lastFrame) * 1000.0;
		lastFrame = currentTime;

		// Update the fps counter every second
		if (currentTime - lastTime >= 1.0) {
			this->fps = frames;
			frames = 0;
			lastTime = currentTime;
		}
	}

	# pragma endregion

	# pragma region Getters

	// Return the GLFWwindow pointer
	GLFWwindow	*Window::getGLFWwindow() const {
		return window;
	}

	// Return the GLFWwindow pointer
	Window::operator GLFWwindow *() const {
		return window;
	}

	// Return the current FPS of the window
	size_t	Window::getFPS() {
		return fps;
	}

	// Return the frameTime of the window
	double	Window::getFrameTime() const {
		return frameTime;
	}

	// Return true if the window is focused
	bool	Window::isFocused() const {
		return glfwGetWindowAttrib(window, GLFW_FOCUSED);
	}

	# pragma endregion

	# pragma region Setters

	// Set the title of the window
	void	Window::setTitle(const std::string &title) {
		glfwSetWindowTitle(window, title.c_str());
	}

	# pragma endregion

} // namespace GE