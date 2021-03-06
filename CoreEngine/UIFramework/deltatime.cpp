#include "stdafx.h"
#include "deltatime.h"
#include <SDL.h>
DeltaTime::DeltaTime(){}
DeltaTime::~DeltaTime(){}

void DeltaTime::start()
{
	if (!m_isStart)
	{
		//<Set Current counter
		m_start = SDL_GetPerformanceCounter();
		m_isStart = true;
	}
}

void DeltaTime::now()
{
	//<Reset start counter
	m_start = m_now;

	//<Set Current counter
	m_now = SDL_GetPerformanceCounter();

}

uint64_t DeltaTime::getDelta()
{
	return static_cast<uint64_t>( (double)((m_now - m_start) * 1000) / SDL_GetPerformanceFrequency() );
}