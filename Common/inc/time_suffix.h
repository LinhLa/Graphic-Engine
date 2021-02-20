#pragma once
#include <chrono>

constexpr std::chrono::milliseconds operator ""_ms(unsigned long long ms)
{
	return std::chrono::milliseconds(ms);
}
constexpr std::chrono::duration<long double, std::milli> operator ""_ms(long double ms)
{
	return std::chrono::duration<long double, std::milli>(ms);
}
