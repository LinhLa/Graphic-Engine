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

TimeLinePlayback::~TimeLinePlayback()
{}

void TimeLinePlayback::onPlay()
{
	auto pObject = m_pObject.lock();
	if (!isStarted())
	{
		start();
	}
	std::chrono::milliseconds currentTime(getMiliseconds());
	if (pObject)
	{
		m_pAnimationProperty->onAnimation(currentTime, pObject);
	}
}

bool TimeLinePlayback::end()
{
	return m_pAnimationProperty->isEnd();
}