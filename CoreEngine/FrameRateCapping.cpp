#include "stdafx.h"
#include "FrameRateCapping.h"
#include <SDL.h>
FrameRateCapping::FrameRateCapping(){}
FrameRateCapping::~FrameRateCapping(){}

void FrameRateCapping::Capping()
{
	//<Calculate Frame Time
	m_FrameTime = SDL_GetTicks() - m_FrameStart;

	if (m_FrameTime < m_delayTime)
	{
		SDL_Delay(m_delayTime - m_FrameTime);
	}
}

void FrameRateCapping::FrameStart(){m_FrameStart = SDL_GetTicks();}