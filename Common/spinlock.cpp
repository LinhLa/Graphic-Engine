#include "stdafx.h"
#include "spinlock.h"


spinlock::spinlock()
{}
spinlock::~spinlock() {}

inline void spinlock::lock()
{
	while (flag.test_and_set(std::memory_order_acquire));
}

inline void spinlock::unlock()
{
	flag.clear(std::memory_order_release);
}


AutoSpinLock::AutoSpinLock(spinlock& t) :m_ref(t)
{
	m_ref.lock();
}

AutoSpinLock::~AutoSpinLock()
{
	m_ref.unlock();
}
