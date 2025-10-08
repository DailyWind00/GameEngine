#pragma once

/// Includes
# include "core/Logger.hpp"

/// System includes
# include <iostream>
# include <algorithm>
# include <sstream>
# include <fstream>
# include <vector>

/// Dependencies
# include <glad/glad.h>
# include <glm/glm.hpp>
# include <glm/gtc/type_ptr.hpp>

namespace GE::OpenGL {
	/// @brief The Shader class is a wrapper around an OpenGL shader program.
	///
	/// It handles the creation, destruction and recompilation of the shader program.
	///
	/// It also provides methods to set uniform variables in the shader program.
	class Shader {
		public:
			Shader(
				const std::string &vertexPath,
				const std::string &fragmentPath,
				const std::string &geometryPath = "",
				Core::Logger *logger = nullptr
			);
			~Shader();

			/// Public functions

			void	use();
			void	recompile();

			/// Uniforms setters

			void    setUniform(const std::string &name, bool value);
			void    setUniform(const std::string &name, int value);
			void    setUniform(const std::string &name, float value);
			void    setUniform(const std::string &name, glm::vec2 value);
			void    setUniform(const std::string &name, glm::vec3 value);
			void    setUniform(const std::string &name, glm::vec4 value);
			void    setUniform(const std::string &name, glm::mat4 value);

			/// Getters

			const GLuint &getID() const;

		private:
			GLuint		shaderID;
			std::string	vertexPath;
			std::string	fragmentPath;
			std::string	geometryPath;

			Core::Logger	*logger = nullptr;

			/// Private functions

			GLuint	make_module(const std::string &filepath, GLuint module_type);
			GLuint	make_shader();
	};
} // namespace GE::OpenGL