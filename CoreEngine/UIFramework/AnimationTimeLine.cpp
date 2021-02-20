#include "stdafx.h"
#include "AnimationTimeLine.h"

AnimationTimeLine::AnimationTimeLine() {}

AnimationTimeLine::~AnimationTimeLine(){}

void AnimationTimeLine::onPlay()
{
	for (auto playbackItr = m_playbackList.begin(); playbackItr != m_playbackList.end();)
	{
		if ((*playbackItr)->end())
		{
			playbackItr = m_playbackList.erase(playbackItr);
		}
		else
		{
			(*playbackItr)->onPlay();
			++playbackItr;
		}
	}
}

void AnimationTimeLine::addTimelinePlayback(TimeLinePlaybackPtr playbackptr)
{
	if (playbackptr)
	{
		m_playbackList.push_back(playbackptr);
	}
}