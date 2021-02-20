#pragma once
#include <cstdio>
#include <iostream>

class Output2File
{
public:
	static FILE*& Stream();
	static void Output(const std::string& msg);
};

