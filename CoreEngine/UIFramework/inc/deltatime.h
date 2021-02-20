#pragma once
#include "SingletonHolder.h"
#include <stdint.h>
class DeltaTime: public SingletonHolder<DeltaTime>
{
private:
	uint64_t m_start = 0;
	uint64_t m_now = 0;
	bool	m_isStart = false;
protected:
	DeltaTime();
	~DeltaTime();
public:
	friend class SingletonHolder<DeltaTime>;
	void start();
	void now();
	uint64_t getDelta();
};