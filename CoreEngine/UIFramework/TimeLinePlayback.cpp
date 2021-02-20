#include "stdafx.h"
#include "TimeLinePlayback.h"

TimeLinePlayback::TimeLinePlayback(UIObjectPtr pObject, AnimationPropertyPtr pAnimationProperty):
	m_pObject(pObject), m_pAnimationProperty(pAnimationProperty)
{
	if (!pObject || !pAnimationProperty)
	{
		throw std::logic_error("invalid argument");
	}
}

TimeLinePlayback::~TimeLinePlayback(){}

void TimeLinePlayback::onPlay()
{
	if (!m_timer.isStarted())
	{
		m_timer.start();
	}
	std::chrono::milliseconds currentTime(m_timer.getMiliseconds());
	m_pAnimationProperty->onAnimation(currentTime, m_pObject);
}

bool TimeLinePlayback::end()
{
	return m_pAnimationProperty->isEnd();
}