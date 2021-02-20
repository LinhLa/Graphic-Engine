#pragma once
#include <iostream>

class GLInfoLog
{
private:
	char m_infoLog[512];
protected:
	GLInfoLog();
	~GLInfoLog();
	const char* GetInfoLog() const;
};
