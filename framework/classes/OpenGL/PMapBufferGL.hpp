#pragma once

/// Defines
# define PERSISTENT_BUFFER_USAGE GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT

/// System includes
# include <iostream>
# include <cstring>

/// Dependencies
# include <glad/glad.h>

namespace GE::OpenGL {
	/// @brief This class is a simple wrapper around OpenGL persistent mapped buffers.
	///
	/// It allows to create, bind, unbind, resize, write and flush buffers.
	///
	/// @note The buffer is mapped persistently, so you can access the data directly via getData().
	/// @note The buffer must be created with GL_MAP_FLUSH_EXPLICIT_BIT in usage if you want to use the flush() method.
	/// @note As this class may be frequently used, it is designed to be as lightweight as possible and have no logging integrated.
	class PMapBufferGL {
		public:
			PMapBufferGL(GLenum type, size_t capacity, GLenum usage = 0);
			~PMapBufferGL();

			/// Public functionsf
			void    bind();
			void    unbind();
			size_t  resize(size_t newCapacity, bool keepData = true);
			bool    write(const void* src, size_t size, size_t offset = 0);
			void    flush(size_t offset = 0, size_t length = 0) const;

			/// Getters
			const void *	getData() const;
			const GLuint &	getID() const;
			const GLenum &	getType() const;
			const size_t &	getCapacity() const;

		private:
			GLuint	_id;
			GLenum	_type;
			GLenum	_usage;
			size_t	_capacity;
			void *	_data;

	};
} // namespace GE::OpenGL