#pragma once

// System includes
# include <type_traits>

namespace GE::Core {
	/**
	 * @brief The Singleton class template provides a way to create a singleton instance of a class.
	 * 
	 * It ensures that only one instance of the class exists throughout the application.
	 * 
	 * The instance is created on first access and is destroyed when the application exits.
	 * 
	 * @tparam T The type of the singleton class.
	 * 
	 * @note This Singleton is thread-safe and supports nothrow default construction. It is designed with C++17 and CRTP (Curiously Recurring Template Pattern) in mind.
	 * 
	 * @see https://github.com/jimmy-park/singleton?tab=readme-ov-file
	 */
	template <typename T>
	class Singleton {
		public:
			// Static method to access the singleton instance
			static T& getInstance() noexcept(std::is_nothrow_default_constructible<T>::value) {
				static T instance;
				return instance;
			}

		protected:
			Singleton() = default;
			Singleton(const Singleton&) = delete;
			Singleton(Singleton&&) = delete;
			Singleton& operator=(const Singleton&) = delete;
			Singleton& operator=(Singleton&&) = delete;
			~Singleton() = default;
	};
} // namespace GE::Core