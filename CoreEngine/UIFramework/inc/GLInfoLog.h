#pragma once
#include <iostream>
#include <cstdint>
class GLInfoLog
{
private:
	char m_infoLog[512];
public:
	GLInfoLog(uint32_t);
	~GLInfoLog();
	const char* GetInfoLog() const;
};
