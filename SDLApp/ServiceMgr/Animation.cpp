#include "stdafx.h"
#include <Animation.h>

Animation::Animation()
{}

Animation::~Animation() {}

void Init(uint32_t framerate, uint32_t maxframe, bool oscillate)
{
	m_FrameRate = framerate;
	m_MaxFrames = maxframe;
	m_Oscillate = oscillate;
}

void Animation::onAnimate()
{
	if ((m_OldTime + m_FrameRate) > SDL_GetTicks())
	{
		return;
	}

	//<Reset old time
	m_OldTime = SDL_GetTicks();

	//increase current frame
	m_CurrentFrame += m_FrameIncrement;

	if (m_Oscillate)
	{
		if (m_FrameIncrement > 0)
		{
			if (m_CurrentFrame >= m_MaxFrames)
			{
				m_FrameIncrement = -m_FrameIncrement;
			}
		}
		else
		{
			if (m_CurrentFrame <= 0)
			{
				m_FrameIncrement = -m_FrameIncrement;
			}
		}
	}
	else
	{
		if (m_CurrentFrame >= m_MaxFrames)
		{
			m_CurrentFrame = 0;
		}
	}
}

void Animation::reset()
{
	m_OldTime = 0;
	m_CurrentFrame = 0;
}

void Animation::SetFrameRate(uint32_t Rate)
{
	m_FrameRate = Rate;
}

uint32_t Animation::GetFrameRate() const
{
	return m_FrameRate;
}

void Animation::SetCurrentFrame(uint32_t Frame)
{
	_ASSERT(Frame < m_MaxFrames);
	m_CurrentFrame = Frame;
}

uint32_t Animation::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

void Animation::SetMaxFrames(uint32_t Frame)
{
	m_MaxFrames = Frame;
}

uint32_t  Animation::GetMaxFrames() const
{
	return m_MaxFrames;
}

void Animation::SetOscillate(bool osc)
{
	m_Oscillate = osc;
}

bool  Animation::GetOscillate() const
{
	return m_Oscillate;
}
