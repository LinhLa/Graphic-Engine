#pragma once
#include <memory>
#include <LifeAndDeathBook.h>
template<class T>
class creator
{
protected:
	creator()
	{
		#ifdef ENABLE_LIFE_AND_DEATH_BOOK
		LifeAndDeathBook::GetInstance()->live(reinterpret_cast<void*>(this));
		#endif
	}
	~creator()
	{
		#ifdef ENABLE_LIFE_AND_DEATH_BOOK
		LifeAndDeathBook::GetInstance()->die(reinterpret_cast<void*>(this));
		#endif
	}
public:
	template<typename... Args>
	static std::shared_ptr<T> create(Args&& ... args)
	{
		return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
	}
};
