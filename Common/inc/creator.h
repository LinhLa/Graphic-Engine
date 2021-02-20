#pragma once
#include <memory>
template<class T>
class creator
{
protected:
	creator(){}
	~creator(){}
public:
	template<typename... Args>
	static std::shared_ptr<T> create(Args&& ... args)
	{
		return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
	}
};
