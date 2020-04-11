#pragma once
#include <spinlock.h>
#include <list>
#include <algorithm>
#include <condition_variable>
#include <chrono>
#include <MyCustomAlloc.h>
#include <time_suffix.h>
template<class T>
struct defaut_destructor
{
	static void destruct(T t) {}
};

template<class T, class Allocator, class Destructor = defaut_destructor<T>>
class queue_protected
{
private:
	std::list<T, Allocator> m_internal_data;
	spinlock m_internal_lock;
	std::condition_variable_any m_condition_any;

public:
	using InternalType		= typename std::list<T, Allocator> ;
	using value_type		= typename InternalType::value_type;
	using iterator			= typename InternalType::iterator;
	using const_iterator	= typename InternalType::const_iterator;

	queue_protected() {}

	queue_protected(const queue_protected& other)
	{
		AutoSpinLock auto_lock(other.m_internal_lock);
		m_internal_data = other.m_internal_data;
	}

	~queue_protected()
	{
		clear();
	}

	void push(const T& _reference)
	{
		//<scope lock
		{
			AutoSpinLock auto_lock(m_internal_lock);
			m_internal_data.push_back(_reference);
		}
		//<notify all waiting thread
		m_condition_any.notify_all();
	}

	bool empty()
	{
		AutoSpinLock auto_lock(m_internal_lock);
		return m_internal_data.empty();
	}

	void clear()
	{
		AutoSpinLock auto_lock(m_internal_lock);
		for(auto &element: m_internal_data)
		{
			Destructor::destruct(element);
		}
		m_internal_data.clear();
	}

	T pop()
	{
		AutoSpinLock auto_lock(m_internal_lock);
		//<queue shouldn't be empty, must be check !empty 1st
		_ASSERT(!m_internal_data.empty());

		T value = m_internal_data.front();
		m_internal_data.pop_front();
		return value;
	}

	void remove(T t)
	{
		AutoSpinLock auto_lock(m_internal_lock);
		auto result = std::find(m_internal_data.begin(), m_internal_data.end(), t);
		if (result != m_internal_data.end())
		{
			Destructor::destruct(*result);
			m_internal_data.erase(result);
		}
	}

	template<class CONDITION>
	void remove_if(CONDITION condition)
	{
		AutoSpinLock auto_lock(m_internal_lock);
		auto _itr = m_internal_data.begin();
		while(_itr !=m_internal_data.end())
		{
			if (condition(*_itr))
			{
				Destructor::destruct(*_itr);
				_itr = m_internal_data.erase(_itr);
				continue;
			}
			++_itr;
		}
	}

	template<class ACTION>
	void do_each(ACTION action)
	{
		AutoSpinLock auto_lock(m_internal_lock);
		auto _itr = m_internal_data.begin();
		while (_itr != m_internal_data.end())
		{
			action(*_itr);
			++_itr;
		}
	}

	queue_protected& operator=(const queue_protected& other)
	{
		if (this == &other)
		{
			return;
		}

		std::lock(m_internal_lock, other.m_internal_lock);
		std::lock_guard(m_internal_lock, std::adopt_lock);
		std::lock_guard(other.m_internal_lock, std::adopt_lock);
		m_internal_data = other.m_internal_data;
	}

	size_t size()
	{
		AutoSpinLock auto_lock(m_internal_lock);
		return m_internal_data.size();
	}

	void wait_until_have_one()
	{
		AutoSpinLock auto_lock(m_internal_lock);
		m_condition_any.wait(m_internal_lock, [&] {return (!m_internal_data.empty()); });
	}

	void wait_until_have_one(uint32_t timeout/*miliseconds*/)
	{
		AutoSpinLock auto_lock(m_internal_lock);
		m_condition_any.wait_until(m_internal_lock, std::chrono::system_clock::now() + timeout * 1_ms, [&] {return (!m_internal_data.empty()); });
	}

	bool wait_for(T t, uint32_t timeout = 300/*miliseconds*/)
	{
		bool bIsQueueHaveOne = false;
		auto findFunct = [&]() -> bool
		{
			if (!m_internal_data.empty())
			{
				auto itr = --m_internal_data.end();
				if (*itr == t)
				{
					bIsQueueHaveOne = true;
				}
			}
			return bIsQueueHaveOne;
		};

		AutoSpinLock auto_lock(m_internal_lock);
		m_condition_any.wait_until(m_internal_lock, std::chrono::system_clock::now() + timeout * 1_ms, findFunct);
		return bIsQueueHaveOne;
	}

	inline void notify_all()
	{
		m_condition_any.notify_all();
	}

	iterator begin() { return m_internal_data.begin(); }
	iterator end() { return m_internal_data.end(); }
};
