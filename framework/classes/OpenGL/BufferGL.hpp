#pragma once

/// System includes
# include <iostream>
# include <vector>

/// Dependencies
# include <glad/glad.h>

namespace GE::OpenGL {
	/// @brief This class is a simple wrapper around OpenGL buffers.
	///
	/// It allows to create, bind, unbind, update and delete buffers.
	///
	/// @note As this class may be frequently used, it is designed to be as lightweight as possible and have no logging integrated.
	class BufferGL {
		private:
			GLuint	_id;
			GLenum	_type;
			GLenum	_usage;
			size_t	_capacity;

		public:
			BufferGL(GLenum type, GLenum usage, size_t size = 0, const void *data = nullptr);
			~BufferGL();

			/// Public functions

			void	bind();
			void	unbind();
			void	updateData(const void *data, size_t size, size_t offset);
			void	resize(size_t newSize, const void *data = nullptr);
			void	clear();

			/// Getters

			const GLuint &	getID() const;
			const GLenum &	getType() const;
			const size_t &	getCapacity() const;
	};
} // namespace GE::OpenGL