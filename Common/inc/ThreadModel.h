#pragma once
/**
* @file ThreadModel.h
*
* @brief template header support general multithread or single thread model for singleton class
*
*
* @ingroup Common.lib
* (Note: this needs exactly one @defgroup somewhere)
*
* @author Linh la
* Contact: lamanhlinh2000@yahoo.com
*
*/

#include "spinlock.h"

enum LOCK_LEVEL
{
	_0 = 0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	_10,
};

/**
* Implementation of template class MultiThreadModel using spinlock with 10 level
*/
template<class T>
class MultiThreadModel
{
protected:
	static spinlock s_mutex[10];
	uint8_t m_locklevel;
public:
	/** Constructor.
	* @brief accquire lock associated with lock level
	* @param locklevel set lock level
	*/
	MultiThreadModel(uint8_t locklevel = 0):m_locklevel(locklevel)
	{
		_ASSERT(m_locklevel <= 10);
		s_mutex[m_locklevel].lock();
	}
	/** destructor.
	* @brief release lock associated with lock level
	*/
	~MultiThreadModel()
	{
		s_mutex[m_locklevel].unlock();
	}
};

template<class T>
spinlock MultiThreadModel<T>::s_mutex[10];

/**
* Implementation of template class SingleThreadModel
*/
template<class T>
class SingleThreadModel
{
public:
	/** Constructor.*/
	SingleThreadModel()
	{
	}
	/** Destructor.*/
	~SingleThreadModel()
	{
	}
};