// LogFramework.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "inc\log.h"

std::string CurrentTime()
{
	std::time_t t = std::time(nullptr);
	std::tm time_struct;
	localtime_s(&time_struct, &t);
	std::stringstream time_string;
	time_string.imbue(std::locale(""));
	time_string << std::put_time(&time_struct, "%c");
	return time_string.str();
}
