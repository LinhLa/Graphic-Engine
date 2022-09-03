#include "stdafx.h"
#include "AnimationProperty.h"
#include "TimeLinePlayback.h"
#include "time_suffix.h"
AnimationProperty::AnimationProperty() {}

AnimationProperty::~AnimationProperty() {}

void AnimationProperty::onAnimation(std::chrono::milliseconds delta, UIObjectPtr pObject, int mode)
{
	for (auto entryItr = m_entryList.begin(); entryItr != m_entryList.end();)
	{
		///< play animation keyframe list
		switch (mode)
		{
		case TIMELINE_PLAYBACK_MODE::_FORWARD:
			entryItr->second->onAnimation(delta, pObject, entryItr->first);
			break;
		case TIMELINE_PLAYBACK_MODE::_BACKWARD:
			entryItr->second->onReverseAnimation(delta, pObject, entryItr->first);
			break;
		default:
			break;
		}
		++entryItr;
	}
}

void AnimationProperty::addEntry(std::string property_name, IKeyFramePtr pKeyframelist)
{
	m_entryList[property_name] = pKeyframelist;
}

bool AnimationProperty::isEnd(std::chrono::milliseconds delta)
{
	bool isAnimationEnd;
	for (auto entryItr = m_entryList.begin(); entryItr != m_entryList.end();)
	{
		bool isEndList = false;
		///<validate keyframe list which over time
		if (entryItr->second->getLastKeyTime() < delta || 0_ms == entryItr->second->getLastKeyTime())
		{
			isEndList = true;
		}
		else {}
		isAnimationEnd &= isEndList;
		++entryItr;
	}
	return isAnimationEnd;
}