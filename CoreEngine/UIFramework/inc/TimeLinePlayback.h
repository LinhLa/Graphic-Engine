#pragma once
#include <memory>
#include "creator.h"
#include "Timer.h"
#include "AnimationProperty.h"
#include "UIObject.h"
class TimeLinePlayback: public creator<TimeLinePlayback>
{
private:
	Timer m_timer;
	UIObjectPtr m_pObject;
	AnimationPropertyPtr m_pAnimationProperty;
protected:
	TimeLinePlayback(UIObjectPtr, AnimationPropertyPtr);
public:
	~TimeLinePlayback();
	friend class creator<TimeLinePlayback>;

	void onPlay();
	bool end();
};

typedef std::shared_ptr<TimeLinePlayback> TimeLinePlaybackPtr;