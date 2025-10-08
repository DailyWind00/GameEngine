#include "BufferGL.hpp"

namespace GE::OpenGL {

	# pragma region Constructors & Destructors

	BufferGL::BufferGL(GLenum type, GLenum usage, size_t size, const void *data)
		: _type(type), _usage(usage), _capacity(size) {
		glGenBuffers(1, &_id);
		glBindBuffer(_type, _id);
		glBufferData(_type, size, data, _usage);
	}

	BufferGL::~BufferGL() {
		if (_id)
			glDeleteBuffers(1, &_id);
	}
	
	# pragma endregion

	# pragma region Public functions

	/// @brief Bind the buffer
	void	BufferGL::bind() {
		glBindBuffer(_type, _id);
	}

	/// @brief Unbind the buffer
	void	BufferGL::unbind() {
		glBindBuffer(_type, 0);
	}

	/// @brief Update the buffer data at the given offset
	/// @param data The new data to update
	/// @param size The size of the data to update
	/// @param offset The offset in the buffer to update
	/// @throw std::out_of_range if the size + offset is greater than the buffer capacity
	void	BufferGL::updateData(const void *data, size_t size, size_t offset) {
		if (size + offset > _capacity)
			throw std::out_of_range("BufferGL: Data overflow - Offset: " + std::to_string(offset) + 
									", Size: " + std::to_string(size) + 
									", Capacity: " + std::to_string(_capacity));
			
		bind();
		glBufferSubData(_type, offset, size, data);
	}

	/// @brief Resize the buffer to the new size
	/// @param newSize The new size of the buffer
	/// @param data Optional data to initialize the buffer with
	/// @note This will reallocate the buffer, losing its previous content
	/// @note This function is heavier than updateData, prefer using updateData if you can
	void BufferGL::resize(size_t newSize, const void *data) {
		_capacity = newSize;

		bind();
		glBufferData(_type, newSize, data, _usage);
	}


	/// @brief Clear the buffer data (set it to zero)
	void	BufferGL::clear() {
		if (!_capacity)
			return;

		bind();
		glBufferData(_type, _capacity, nullptr, _usage);
	}
	
	# pragma endregion

	# pragma region Getters

	/// @brief Return the buffer ID
	/// @return The OpenGL buffer ID
	const GLuint	&BufferGL::getID() const {
		return _id;
	}

	/// @brief Return the buffer type
	/// @return The OpenGL buffer type (e.g. GL_ARRAY_BUFFER)
	const GLenum	&BufferGL::getType() const {
		return _type;
	}

	/// @brief Return the buffer capacity
	/// @return The capacity of the buffer in bytes
	const size_t	&BufferGL::getCapacity() const {
		return _capacity;
	}

	# pragma endregion

} // namespace GE::OpenGL