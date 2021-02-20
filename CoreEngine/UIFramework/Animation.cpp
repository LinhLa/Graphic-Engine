#include "stdafx.h"
#include <Animation.h>

SpriteSheetAnimation::SpriteSheetAnimation()
{}

SpriteSheetAnimation::~SpriteSheetAnimation() {}

void SpriteSheetAnimation::Init(uint32_t framerate, uint32_t maxframe, bool oscillate)
{
	m_FrameRate = framerate;
	m_MaxFrames = maxframe;
	m_Oscillate = oscillate;
}

void SpriteSheetAnimation::onAnimate()
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

void SpriteSheetAnimation::reset()
{
	m_OldTime = 0;
	m_CurrentFrame = 0;
}

void SpriteSheetAnimation::SetFrameRate(uint32_t Rate)
{
	m_FrameRate = Rate;
}

uint32_t SpriteSheetAnimation::GetFrameRate() const
{
	return m_FrameRate;
}

void SpriteSheetAnimation::SetCurrentFrame(uint32_t Frame)
{
	_ASSERT(Frame < m_MaxFrames);
	m_CurrentFrame = Frame;
}

uint32_t SpriteSheetAnimation::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

void SpriteSheetAnimation::SetMaxFrames(uint32_t Frame)
{
	m_MaxFrames = Frame;
}

uint32_t  SpriteSheetAnimation::GetMaxFrames() const
{
	return m_MaxFrames;
}

void SpriteSheetAnimation::SetOscillate(bool osc)
{
	m_Oscillate = osc;
}

bool  SpriteSheetAnimation::GetOscillate() const
{
	return m_Oscillate;
}
