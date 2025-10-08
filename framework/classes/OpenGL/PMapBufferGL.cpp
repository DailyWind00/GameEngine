#include "PMapBufferGL.hpp"

namespace GE::OpenGL {

	# pragma region Constructors & Destructors

	PMapBufferGL::PMapBufferGL(GLenum type, size_t capacity, GLenum usage)
		: _type(type), _usage(usage), _capacity(capacity), _data(nullptr) {
		
		glGenBuffers(1, &_id);
		glBindBuffer(_type, _id);

		glBufferStorage(type, capacity, nullptr, PERSISTENT_BUFFER_USAGE);

		_data = glMapBufferRange(type, 0, capacity, _usage);

		if (!_data)
			throw std::runtime_error("PMapBufferGL: Failed to map the buffer");
	}

	PMapBufferGL::~PMapBufferGL() {
		if (_data)
			glUnmapBuffer(_type);
	}
	
	# pragma endregion

	# pragma region Public functions

	/// @brief Bind the buffer
	void PMapBufferGL::bind() {
		glBindBuffer(_type, _id);
	}

	/// @brief Unbind the buffer
	void PMapBufferGL::unbind() {
		glBindBuffer(_type, 0);
	}

	/// @brief Flush the buffer to the GPU
	/// @param offset The offset to start flushing from
	/// @param length The length to flush, if 0, flushes until the end of the buffer
	/// @throw std::runtime_error if the buffer was not created with GL_MAP_FLUSH_EXPLICIT_BIT
	void PMapBufferGL::flush(size_t offset, size_t length) const {
		if (!(_usage & GL_MAP_FLUSH_EXPLICIT_BIT))
			throw std::runtime_error("PMapBufferGL: Buffer not created with GL_MAP_FLUSH_EXPLICIT_BIT flag");

		if (offset >= _capacity)
			return;

		if (!length || offset + length > _capacity)
			length = _capacity - offset;

		glFlushMappedBufferRange(_type, offset, length);
	}

	/// @brief Resize the buffer to the new capacity
	/// @param newCapacity The new capacity of the buffer
	/// @param keepData Whether to keep the current data in the buffer
	/// @note This function recreates the buffer, so you need to update your buffer attributes if you use them elsewhere
	/// @return The new capacity of the buffer, or 0 if the resize failed
	size_t PMapBufferGL::resize(size_t newCapacity, bool keepData) {
		if (newCapacity < _capacity)
			return _capacity;

		// Copy current data
		uint8_t *	copy = nullptr;
		if (keepData) {
			copy = new uint8_t[newCapacity];
			std::memset(copy, 0, newCapacity);
			std::memcpy(copy, _data, _capacity);
		}

		// Delete current buffer
		glUnmapBuffer(_type);
		glDeleteBuffers(1, &_id);

		// Create a new buffer
		glGenBuffers(1, &_id);
		glBindBuffer(_type, _id);
		glBufferStorage(_type, newCapacity, copy, PERSISTENT_BUFFER_USAGE);
		_data = glMapBufferRange(_type, 0, newCapacity, _usage);

		delete[] static_cast<uint8_t*>(copy);

		if (!_data)
			return _capacity = 0;

		return _capacity = newCapacity;
	}

	/// @brief Write data to the buffer at the given offset
	/// @param data The source data to write
	/// @param size The size of the data to write
	/// @param offset The offset in the buffer to write to
	/// @return True if the write was successful, false otherwise
	/// @note If src is nullptr, the buffer will be filled with zeros
	/// @note This function does not flush the buffer, you need to call flush() if needed
	bool PMapBufferGL::write(const void* data, size_t size, size_t offset) {
		if (offset + size > _capacity)
			return false;

		if (!data)
			std::memset(static_cast<uint8_t*>(_data) + offset, 0, size);
		else
			std::memcpy(static_cast<uint8_t*>(_data) + offset, data, size);

		return true;
	}
	
	# pragma endregion

	# pragma region Getters

	/// @brief Return a pointer to the buffer data
	/// @return The pointer to the buffer data
	const void * PMapBufferGL::getData() const {
		return _data;
	}

	/// @brief Return the buffer ID
	/// @return The buffer ID
	const GLuint & PMapBufferGL::getID() const {
		return _id;
	}

	/// @brief Return the buffer type
	/// @return The buffer type (e.g. GL_ARRAY_BUFFER)
	const GLenum & PMapBufferGL::getType() const {
		return _type;
	}

	/// @brief Return the buffer capacity
	/// @return The buffer capacity in bytes
	const size_t & PMapBufferGL::getCapacity() const {
		return _capacity;
	}
	
	# pragma endregion

} // namespace GE::OpenGL