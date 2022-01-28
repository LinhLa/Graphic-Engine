#pragma once
#include "SingletonHolder.h"
#include <stdint.h>
class DeltaTime: public SingletonHolder<DeltaTime>
{
private:
	uint64_t m_start = 0U;
	uint64_t m_now = 0U;
	bool	m_isStart = false;
protected:
	DeltaTime();
	~DeltaTime();
public:
	friend class SingletonHolder<DeltaTime>;
	void start();
	void now();

	double getDelta();
};