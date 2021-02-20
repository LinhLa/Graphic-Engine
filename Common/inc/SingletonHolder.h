#pragma once
#include <mutex>
#include <atomic>
template<class T>
class SingletonHolder
{
private:
	static std::once_flag m_init_flag;
	static std::atomic<T*> m_pInstance;

	static void Init()
	{
		static T instance;
		m_pInstance.store(&instance, std::memory_order_release);
	}

public:
	static T* GetInstance()
	{
		if (nullptr == m_pInstance.load(std::memory_order_acquire))
		{
			std::call_once(m_init_flag, SingletonHolder::Init);
		}
		return m_pInstance;
	}
};

template<class T>
std::atomic<T*> SingletonHolder<T>::m_pInstance = nullptr;

template<class T>
std::once_flag SingletonHolder<T>::m_init_flag;