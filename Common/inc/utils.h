#pragma once
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <vector>

#include <glm/glm.hpp>
#include <glm/common.hpp>

namespace util
{
	template<typename Condition>
	void wait_for(std::chrono::milliseconds sleep_for, std::chrono::milliseconds time_out, Condition condt)
	{
		auto start = std::chrono::steady_clock::now();
		std::chrono::duration<double> diff;
		while (!condt())
		{
			auto end = std::chrono::steady_clock::now();
			diff = end - start;
			if ((diff.count() * 1000/*from seconds to milisecond*/) >= time_out)
			{
				return;
			}

			if (sleep_for.count())
			{
				std::this_thread::sleep_for(sleep_for);
			}
			else
			{
				std::this_thread::yield();
			}
		}
	}

	template<typename Condition>
	void wait_for(std::chrono::milliseconds sleep_for, Condition condt)
	{
		while (!condt())
		{
			if (sleep_for.count())
			{
				std::this_thread::sleep_for(sleep_for);
			}
			else
			{
				std::this_thread::yield();
			}
		}
	}

	template<class T>
	T ConvertToType(std::string& strVal)
	{
		std::stringstream sstream(strVal);
		T value;
		sstream >> value;
		return value;
	}

	template<class T>
	typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
		almost_equal(T x, T y, int ulp)
	{
		// the machine epsilon has to be scaled to the magnitude of the values used
		// and multiplied by the desired precision in ULPs (units in the last place)
		return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
			// unless the result is subnormal
			|| std::fabs(x - y) < std::numeric_limits<T>::min();
	}
}