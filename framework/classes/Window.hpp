# pragma once

/// Defines
# define FAILURE -1

/// Includes
# include "Logger.hpp"

/// System includes
# include <string>
# include <vector>

/// Dependencies
# include <glad/glad.h>
# include <glfw/glfw3.h>

namespace GE {
	/// @brief The Window class is a wrapper around a GLFW window.
	/// It handles the creation, destruction and main loop of the window.
	/// It also provides some useful getters and setters.
	class Window {
		public:
			Window(
				int posX,
				int posY,
				int width,
				int height,
				const std::string &title,
				const float &GLversion = 4.2f,
				Core::Logger *logger = nullptr
			);
			~Window();

			/// Public functions

			/// @brief Main loop of the window, calls the given function each frame with the given arguments.
			/// @tparam ...T Types of the arguments to pass to the function.
			/// @param func Pointer to the function to call each frame.
			/// @param ...args Arguments to pass to the function.
			/// @note The function polls events, updates frame time, clear and swaps buffers automatically.
			template <typename ...T>
			void	mainLoop(void (*func)(T&...), T&... args) {
				if (logger) logger->info("Window : Entering loop");

				while (!glfwWindowShouldClose(window)) {
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					this->updateFrameRate();

					func(args...);

					glfwPollEvents();
					glfwSwapBuffers(window);
				}

				if (logger) logger->info("Window : Exiting loop");
			}

			/// Getters

			GLFWwindow *getGLFWwindow() const;
			operator	GLFWwindow *() const;

			size_t	getFPS();
			double	getFrameTime() const;
			bool	isFocused() const;

			/// Setters

			void	setTitle(const std::string &title);

		private:
			Core::Logger	*logger = nullptr;
			GLFWwindow *window;

			// Only used for the main loop
			size_t		fps = 0;
			double		frameTime = 0;

			/// Private functions

			void	updateFrameRate();
	};
} // namespace GE