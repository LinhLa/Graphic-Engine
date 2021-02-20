#include <vector>
#include <assert.h>
/**
 * @brief      The Pool class is the most important class in the object pool design pattern.
 * ObjectPool maintains a list of available objects and a collection of objects that have already been requested from the pool.
 *
 * @tparam     T        { object }
 */
template<class T>
class ObjectPool
{
protected:
	std::vector<T*> mPool;
public:
	ObjectPool(){}

	virtual ~ObjectPool()
	{
		for(auto &o : mPool)
		{
			if (o)
				delete o;
		}
		mPool.clear();
	}

	/**
	 * @brief      { get object }
	 *
	 * @return     { return pointer to object }
	 */
	T* Get()
	{
		T *t = nullptr;
		if (!mPool.empty())
		{
			t = mPool.back();
			mPool.pop_back();
		}
		else
		{
			// no objects available, create a new one
			t = Create();
		}
		return t;
	}

	/**
	 * @brief      { retur object }
	 *
	 * @param      t     { object }
	 */
	void Push(T *t)
	{
		assert(t);
		mPool.push_back(t);
	}


	/**
	 * @brief      { get current number of object in pool }
	 *
	 * @return     { number of object }
	 */
	size_t size()
	{
		return mPool.size();
	}

protected:
	/**
	 * @brief      { creation method }
	 *
	 * @return     { pointer to object }
	 */
	virtual T* Create() = 0;
};