#pragma once
#include "log.h"
#include "memmgr.h"

/**
 * @brief      This class describes my custom dynamic allocate.
 *
 * @tparam     T     { description }
 */
template <class T>
class MyCustomDynamicAlloc
{
private:
	DynamicMemorySizeManager *m_pMemory = nullptr;

public:

	typedef T          value_type;
	typedef size_t     size_type;

	typedef ptrdiff_t  difference_type;
	typedef T*         pointer;
	typedef const T*   const_pointer;

	typedef T&         reference;
	typedef const T&   const_reference;

	MyCustomDynamicAlloc() :m_pMemory(DynamicMemorySizeManager::GetInstance())
	{
		_ASSERT(m_pMemory);
	}

	/**
	 * @brief      Constructs a new instance.
	 *
	 * @param[in]  obj   The object
	 */
	MyCustomDynamicAlloc(const MyCustomDynamicAlloc &obj) // copy constructor
	{
		m_pMemory = obj.m_pMemory;
		_ASSERT(m_pMemory);
	}

public:

	/**
	 * @brief      Assignment operator.
	 *
	 * @param[in]  other  The other
	 */
	inline void operator=(const MyCustomDynamicAlloc &other)
	{
		m_pMemory = other.m_pMemory;
		_ASSERT(m_pMemory);
	}

	/**
	 * @brief      Gets the memory.
	 *
	 * @return     The memory.
	 */
	DynamicMemorySizeManager* GetMemory() const
	{
		return m_pMemory;
	}

	template <class _Other>
	MyCustomDynamicAlloc(const MyCustomDynamicAlloc<_Other> &other)
	{
		m_pMemory = other.GetMemory();
		_ASSERT(m_pMemory);
	}

	/**
	 * @brief      Destroys the object.
	 */
	~MyCustomDynamicAlloc()
	{
	}

	/**
	 * @brief      { struct_description }
	 *
	 * @tparam     U     { description }
	 */
	template <class U>
	struct rebind
	{
		typedef MyCustomDynamicAlloc<U> other;
	};

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  r     { parameter_description }
	 *
	 * @return     { description_of_the_return_value }
	 */
	pointer address(reference r) const
	{
		return &r;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  r     { parameter_description }
	 *
	 * @return     The constant pointer.
	 */
	const_pointer address(const_reference r) const
	{
		return &r;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  n          { parameter_description }
	 * @param[in]  <unnamed>  { parameter_description }
	 *
	 * @return     { description_of_the_return_value }
	 */
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

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  p     { parameter_description }
	 * @param[in]  size  The size
	 */
	inline void deallocate(pointer p, size_type size)
	{
#ifdef __MEMMGR_H_
		m_pMemory->deallocate(p);
#else
		free(p);
#endif

	}

	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  p     { parameter_description }
	 * @param[in]  val   The value
	 */
	void construct(pointer p, const T& val)
	{
		new (p) T(val);
	}

	void destroy(pointer p)
	{
		p->~T();
	}

	/**
	 * @brief      { function_description }
	 *
	 * @return     The size type.
	 */
	size_type max_size() const
	{
		return ULONG_MAX / sizeof(T);
	}
};