#pragma once
#include <cstdio>
#include <iostream>

class file_guard
{
private:
	FILE* pstream = nullptr;
public:
	file_guard(const char* path, const char* mode);
	~file_guard();
	FILE*& stream();
};
