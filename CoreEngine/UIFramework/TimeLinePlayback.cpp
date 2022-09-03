#include "stdafx.h"
#include "TimeLinePlayback.h"
#include "AnimationTimeLine.h"

TimeLinePlayback::TimeLinePlayback(UIObjectPtr pObject, AnimationPropertyPtr pAnimationProperty):
	m_pObject(pObject), m_pAnimationProperty(pAnimationProperty)
{
	if (!pObject || !pAnimationProperty)
	{
		throw std::logic_error("invalid argument");
	}
}

TimeLinePlayback::~TimeLinePlayback()
{}

void TimeLinePlayback::setStartTime(std::chrono::milliseconds startTime)
{
	m_startTime = startTime;
}

std::chrono::milliseconds TimeLinePlayback::getStartTime()
{
	return m_startTime;
}

void TimeLinePlayback::onPlay(std::chrono::milliseconds delta)
{
	auto pObject = m_pObject.lock();
	if (pObject)
	{
		m_pAnimationProperty->onAnimation(m_progress + delta, pObject, m_mode);
	}
}

bool TimeLinePlayback::end(std::chrono::milliseconds delta)
{
	return m_pAnimationProperty->isEnd(m_progress + delta);
}

void TimeLinePlayback::setCallbackComplete(callbackType callback)
{
	m_onCompleted = callback;
}

TIMELINE_PLAYBACK_STATE TimeLinePlayback::getState() 
{
	return m_state;
}

std::chrono::milliseconds TimeLinePlayback::getProgress()
{
	return std::chrono::milliseconds(AnimationTimeLine::GetInstance()->getMiliseconds()) - m_startTime;
}

void TimeLinePlayback::setProgress(std::chrono::milliseconds progress)
{
	m_progress = progress;
}

void TimeLinePlayback::setPlaybackMode(TIMELINE_PLAYBACK_MODE mode)
{
	m_mode = mode;
}