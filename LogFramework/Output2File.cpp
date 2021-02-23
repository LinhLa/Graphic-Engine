#include "stdafx.h"
#include "..\Common\inc\file_guard.h"
#include "inc\Output2File.h"


void Output2File::Output(const std::string& msg)
{
	FILE* pStream = Stream();
	if (!pStream)
		return;
	fprintf(pStream, "%s", msg.c_str());
	fflush(pStream);
}

FILE*& Output2File::Stream()
{
	static file_guard file(".\\log.txt", "ab");
	return file.stream();
}
