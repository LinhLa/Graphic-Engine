#pragma once
/**
* @file spinlock.h
*
* @brief template header implement lightweigh lock using atomic flag
*
* @ingroup Common.lib
* (Note: this needs exactly one @defgroup somewhere)
*
* @author Linh la
* Contact: lamanhlinh2000@yahoo.com
*
*/
#include <atomic>
class spinlock
{
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
	spinlock();
	~spinlock();

	inline void lock();

	inline void unlock();
};

class AutoSpinLock
{
private:
	spinlock &m_ref;
public:
	AutoSpinLock(spinlock& t);
	~AutoSpinLock();
};