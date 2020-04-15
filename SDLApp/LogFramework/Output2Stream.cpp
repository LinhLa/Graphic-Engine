#include "stdafx.h"
#include "inc\Output2Stream.h"

void Output2Stream::Output(const std::string& msg)
{
	FILE* pStream = Stream();
	if (!pStream)
		return;
	fprintf(pStream, "%s", msg.c_str());
	fflush(pStream);
}

FILE*& Output2Stream::Stream()
{
	static FILE* pStream = stderr;
	return pStream;
}
