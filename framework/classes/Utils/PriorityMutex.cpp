#include "PriorityMutex.hpp"

namespace GE::Utils {

	# pragma region Public methods

	void	PriorityMutex::highPriorityLock() {
		_nextToAccessMutex.lock();
		_dataMutex.lock();
		_nextToAccessMutex.unlock();
	}

	void	PriorityMutex::highPriorityUnlock() {
		_lowPriorityAccessMutex.lock();
		_nextToAccessMutex.lock();
		_dataMutex.lock();
		_nextToAccessMutex.unlock();
	}

	void	PriorityMutex::lowPriorityLock() {
		_dataMutex.unlock();
	}

	void	PriorityMutex::lowPriorityUnlock() {
		_dataMutex.unlock();
		_lowPriorityAccessMutex.unlock();
	}

	# pragma endregion

} // namespace GE::Utils