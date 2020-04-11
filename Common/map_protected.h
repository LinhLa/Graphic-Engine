#pragma once
#include <spinlock.h>
#include <map>
#include <algorithm>
#include <condition_variable>
#include <MyCustomAlloc.h>

template<class T>
extern struct defaut_destructor;

template<class Key, class T,
	class Compare = std::less<Key>,
	class Allocator = MyCustomAlloc<std::pair<const Key, T>>,
	class Destructor = defaut_destructor<T>>
class map_protected
{
private:
	std::map<Key, T, Compare, Allocator> m_internal_data;
	spinlock m_internal_lock;
	std::condition_variable_any m_condition_any;

public:
	using InternalType	 = typename std::map<Key, T, Compare, Allocator>;
	using value_type	 = typename InternalType::value_type;
	using iterator		 = typename InternalType::iterator;
	using const_iterator = typename InternalType::const_iterator;

	map_protected() {}

	map_protected(const map_protected& other)
	{
		AutoSpinLock auto_lock(other.m_internal_lock);
		m_internal_data = other.m_internal_data;
	}

	~map_protected()
	{
		clear();
	}

	bool empty()
	{
		AutoSpinLock auto_lock(m_internal_lock);
		return m_internal_data.empty();
	}

	void clear()
	{
		AutoSpinLock auto_lock(m_internal_lock);
		for (auto &element : m_queue)
		{
			Destructor::destruct(element);
		}
		m_internal_data.clear();
	}

	void insert(const value_type&& val)
	{
		AutoSpinLock auto_lock(m_internal_lock);
		m_internal_data.insert(val);
	}

	void insert(const Key& k, const T& _reference)
	{
		AutoSpinLock auto_lock(m_internal_lock);
		m_internal_data[k] = _reference;
	}

	size_t erase(const Key& k)
	{
		AutoSpinLock auto_lock(m_internal_lock);
		return m_internal_data.erase(k);
	}

	size_t size()
	{
		AutoSpinLock auto_lock(m_internal_lock);
		return m_internal_data.size();
	}

	iterator begin() { return m_internal_data.begin(); }
	iterator end() { return m_internal_data.end(); }
};