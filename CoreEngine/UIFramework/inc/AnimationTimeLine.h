#pragma once
#include <list>
#include "SingletonHolder.h"
#include "TimeLinePlayback.h"
class AnimationTimeLine: public SingletonHolder<AnimationTimeLine>
{
private:
	friend class SingletonHolder<AnimationTimeLine>;
	std::list<TimeLinePlaybackPtr>		m_playbackList;
protected:
	AnimationTimeLine();
	~AnimationTimeLine();
public:
	void onPlay();
	void addTimelinePlayback(TimeLinePlaybackPtr);
};