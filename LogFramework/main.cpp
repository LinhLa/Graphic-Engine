// LogFramework.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <log.h>
#include <thread_guard.h>

void thread_log()
{
	std::ostringstream os;
	os << std::this_thread::get_id();
	FUNC_IN("Thread[%s]", os.str().c_str());
	for (size_t i = 0; i < 1000; ++i)
	{
		LOG_INFO("the counter i = %u", i);
	}
	FUNC_OUT("");
}

int main(int argc, char* argv[])
{
	FUNC_IN("");
	thread_guard<Join> g(std::thread(thread_log));
	try
	{
		const int count = 3;
		LOG_DEBUG("A loop with %d iterations", count);
		for (int i = 0; i != count; ++i)
		{
			LOG_INFO("the counter i = %d", i);
		}
	}
	catch (const std::exception& e)
	{
		LOG_ERROR("%s", e.what());
	}
	FUNC_OUT("");
	getchar();
	return 0;
}
