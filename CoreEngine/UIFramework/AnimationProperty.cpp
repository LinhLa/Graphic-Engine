#include "stdafx.h"
#include "AnimationProperty.h"
#include "time_suffix.h"
AnimationProperty::AnimationProperty() {}

AnimationProperty::~AnimationProperty(){}

void AnimationProperty::onAnimation(std::chrono::milliseconds time, UIObjectPtr pObject)
{
	for (auto entryItr = m_entryList.begin(); entryItr != m_entryList.end();)
	{
		///< play animation keyframe list
		entryItr->second->onAnimation(time, pObject, entryItr->first);

		///<remove keyframe list which over time
		if (entryItr->second->getLastKeyTime() < time || 0_ms == entryItr->second->getLastKeyTime())
		{
			entryItr = m_entryList.erase(entryItr);
		}
		else
		{
			++entryItr;
		}
	}
}

void AnimationProperty::addEntry(std::string property_name, IKeyFramePtr pKeyframelist)
{
	m_entryList[property_name] = pKeyframelist;
}

bool AnimationProperty::isEnd()
{
	return m_entryList.empty();
}