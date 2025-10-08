#pragma once

/// Includes
# include "core/Logger.hpp"

/// System includes
# include <iostream>
# include <vector>

/// Dependencies
# include <glad/glad.h>
# include <glfw/glfw3.h>
# include "stb_image/stb_image.h"

namespace GE::Objects {
	/**
	 * @brief Class to create a skybox as a cube with a cubemap texture.
	 * 
	 * The SkyBox class handles the creation, rendering, and destruction of a skybox object.
	 * 
	 * It can loads six textures to create a cubemap and sets up the necessary OpenGL buffers
	 * to render the skybox as a cube.
	 * 
	 * The order of the textures should be:
	 * +X (right), -X (left), +Y (top), -Y (bottom), +Z (front), -Z (back).
	 * 
	 * @note The SkyBox class does not include shader management. You need to provide your own shaders to render the skybox.
	 * @note Texture are optional and can be replaced by a shader-only skybox.
	 */
	class	SkyBox {
		private:
			GLuint	VAO;
			GLuint	VBO;
			GLuint	textureID = 0;
			Core::Logger	*logger = nullptr;

		public:
			SkyBox(const std::vector<std::string> &path = {}, Core::Logger *logger = nullptr);
			~SkyBox();

			/// Public functions

			void	draw();
	};
}


