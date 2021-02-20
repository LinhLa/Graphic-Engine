#include "stdafx.h"
#include "inc\file_guard.h"

file_guard::file_guard(const char* path, const char* mode)
{
	try {
#if defined(_WIN32)
		fopen_s(&pstream, path, mode);
#else
		pstream = fopen(path, mode);
#endif
	}
	catch (std::exception &ex)
	{
		std::cout << ex.what();
	}
}

file_guard::~file_guard()
{
	if (pstream)
	{
		fclose(pstream);
	}
}

FILE*& file_guard::stream() { return pstream; }
