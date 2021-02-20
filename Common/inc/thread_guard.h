#pragma once
#include <thread>

struct detach
{
	inline static void init(std::thread& t) {
		t.detach();
	}
};

struct Join
{
	inline void init(std::thread& t) { t.join(); }
};

template<class InitThread>
class thread_guard
{
	std::thread& t;
public:
	explicit thread_guard(std::thread& t_) : t(t_) {}
	~thread_guard()
	{
		if (t.joinable())
		{
			InitThread::init(t);
		}
	}
	thread_guard(thread_guard const&) = delete;
	thread_guard& operator=(thread_guard const&) = delete;
};