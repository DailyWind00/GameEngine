# pragma once

/// System includes
# include <mutex>

namespace GE::Utils {
	/**
	 * @brief A mutex that allows prioritizing high-priority locks over low-priority locks.
	 * 
	 * This is useful in scenarios where certain operations must not be delayed by less critical ones.
	 * 
	 * The PriorityMutex class provides methods to lock and unlock the mutex with high or low priority,
	 * High-priority locks will always be granted access before low-priority locks.
	 * 
	 * @see https://stackoverflow.com/questions/11666610/how-to-give-priority-to-privileged-thread-in-mutex-locking
	 */
	class PriorityMutex {
		public:
			PriorityMutex() = default;
			~PriorityMutex() = default;

			/// Public methods

			void	highPriorityLock();
			void	highPriorityUnlock();
			void	lowPriorityLock();
			void	lowPriorityUnlock();

		private:
			std::mutex	_lowPriorityAccessMutex;
			std::mutex	_nextToAccessMutex;
			std::mutex	_dataMutex;
	};
} // namespace GE::Utils