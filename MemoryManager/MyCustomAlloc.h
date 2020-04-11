#pragma once
#include "log.h"
#include "memmgr.h"

template <class T>
class MyCustomAlloc
{
private:
	FreeListMemoryManager *m_pMemory = nullptr;
public:

	typedef T          value_type;
	typedef size_t     size_type;

	typedef ptrdiff_t  difference_type;
	typedef T*         pointer;
	typedef const T*   const_pointer;

	typedef T&         reference;
	typedef const T&   const_reference;

	MyCustomAlloc() :m_pMemory(FreeListMemoryManager::GetInstance())
	{
		_ASSERT(m_pMemory);
	}

	MyCustomAlloc(const MyCustomAlloc &obj) // copy constructor
	{
		m_pMemory = obj.m_pMemory;
		_ASSERT(m_pMemory);
	}

public:
	inline void operator=(const MyCustomAlloc &other)
	{
		m_pMemory = other.m_pMemory;
		_ASSERT(m_pMemory);
	}

	FreeListMemoryManager* GetMemory() const
	{
		return m_pMemory;
	}

	template <class _Other>
	MyCustomAlloc(const MyCustomAlloc<_Other> &other)
	{
		m_pMemory = other.GetMemory();
		_ASSERT(m_pMemory);
	}

	~MyCustomAlloc()
	{
	}

	template <class U>
	struct rebind
	{
		typedef MyCustomAlloc<U> other;
	};

	pointer address(reference r) const
	{
		return &r;
	}

	const_pointer address(const_reference r) const
	{
		return &r;
	}

	inline pointer allocate(size_type n, const void* /*hint*/ = 0)
	{
		FUNC_IN("size_type[%u]", n * sizeof(T));
		size_t block_size = n * sizeof(T);
#ifdef __MEMMGR_H_
		pointer p = pointer(m_pMemory->allocate(block_size));
#else
		pointer p = malloc(n * sizeof(T));
#endif

		FUNC_OUT("return[%p]", p);
		return p;
	}

	inline void deallocate(pointer p, size_type size)
	{
#ifdef __MEMMGR_H_
		m_pMemory->deallocate(p);
#else
		free(p);
#endif

	}

	void construct(pointer p, const T& val)
	{
		new (p) T(val);
	}

	void destroy(pointer p)
	{
		p->~T();
	}

	size_type max_size() const
	{
		return ULONG_MAX / sizeof(T);
	}
};

template <class T>
bool operator==(const MyCustomAlloc<T>& left, const MyCustomAlloc<T>& right)
{
	return (left.GetMemory() == right.GetMemory());
}

template <class T>
bool operator!=(const MyCustomAlloc<T>& left, const MyCustomAlloc<T>& right)
{
	return (left.GetMemory() != right.GetMemory());
}
// </MyCustomAlloc>
