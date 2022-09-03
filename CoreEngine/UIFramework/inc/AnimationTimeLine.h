#pragma once
#include <list>
#include "SingletonHolder.h"
#include "TimeLinePlayback.h"
#include "Timer.h"
class AnimationTimeLine: public SingletonHolder<AnimationTimeLine>, public Timer
{
private:
	friend class SingletonHolder<AnimationTimeLine>;
	std::list<TimeLinePlaybackPtr>		m_playbackList;
protected:
	AnimationTimeLine();
	~AnimationTimeLine();
public:
	void onPlay();
	void start();
	void addTimelinePlayback(TimeLinePlaybackPtr);
	void removeTimelinePlayback(TimeLinePlaybackPtr);
};