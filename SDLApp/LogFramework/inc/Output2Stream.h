#pragma once
#include <cstdio>
#include <iostream>
class Output2Stream
{
public:
	static FILE*& Stream();
	static void Output(const std::string& msg);
};