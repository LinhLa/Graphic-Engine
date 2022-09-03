#include "stdafx.h"
#include "AnimationTimeLine.h"
#include "Timer.h"

AnimationTimeLine::AnimationTimeLine() {}

AnimationTimeLine::~AnimationTimeLine(){}

void AnimationTimeLine::start()
{
	if (!isStarted())
	{
		Timer::start();
	}
}

void AnimationTimeLine::onPlay()
{
	std::chrono::milliseconds current_time(getMiliseconds());
	for (auto playbackItr = m_playbackList.begin(); playbackItr != m_playbackList.end();)
	{
		if ((*playbackItr)->end(current_time))
		{
			//playback completed
			(*playbackItr)->m_state = TIMELINE_PLAYBACK_STATE::_FINISHED;
			auto callbackComplete = (*playbackItr)->m_onCompleted;
			if (nullptr != callbackComplete)
			{
				auto playback = *playbackItr;
				callbackComplete(playback);
			}
			playbackItr = m_playbackList.erase(playbackItr);
		}
		else
		{
			(*playbackItr)->onPlay(current_time - (*playbackItr)->getStartTime());
			++playbackItr;
		}
	}
}

void AnimationTimeLine::addTimelinePlayback(TimeLinePlaybackPtr playbackptr)
{
	if (playbackptr)
	{
		playbackptr->setStartTime(std::chrono::milliseconds(getMiliseconds()));
		m_playbackList.push_back(playbackptr);
		playbackptr->m_state = TIMELINE_PLAYBACK_STATE::_STARTED;
	}
}

void AnimationTimeLine::removeTimelinePlayback(TimeLinePlaybackPtr playbackptr)
{
	if (playbackptr)
	{
		m_playbackList.remove(playbackptr);
	}
}