#pragma once
#include <memory>
#include "creator.h"
#include "AnimationProperty.h"
#include "UIObject.h"
enum TIMELINE_PLAYBACK_STATE
{
	_NOT_STARTED,
	_STARTED,
	_FINISHED,
};

enum TIMELINE_PLAYBACK_MODE
{
	_FORWARD,
	_BACKWARD,
	_INFINITY,
};
class AnimationTimeLine;
class TimeLinePlayback: public creator<TimeLinePlayback>
{
private:
	friend class AnimationTimeLine;
	std::function<void(std::shared_ptr<TimeLinePlayback>)> m_onCompleted = nullptr;
	std::weak_ptr<UIObject> m_pObject;
	std::chrono::milliseconds m_startTime;
	std::chrono::milliseconds m_progress = std::chrono::milliseconds::zero();
	AnimationPropertyPtr m_pAnimationProperty;

	TIMELINE_PLAYBACK_STATE m_state = _NOT_STARTED;
	TIMELINE_PLAYBACK_MODE m_mode = _FORWARD;
protected:
	TimeLinePlayback(UIObjectPtr, AnimationPropertyPtr);
public:
	typedef std::function<void(std::shared_ptr<TimeLinePlayback>)> callbackType;
	~TimeLinePlayback();
	friend class creator<TimeLinePlayback>;

	void onPlay(std::chrono::milliseconds);
	void setStartTime(std::chrono::milliseconds);
	std::chrono::milliseconds getStartTime();
	bool end(std::chrono::milliseconds);

	void setCallbackComplete(callbackType);
	TIMELINE_PLAYBACK_STATE getState();

	std::chrono::milliseconds getProgress();
	void setProgress(std::chrono::milliseconds);

	void setPlaybackMode(TIMELINE_PLAYBACK_MODE);
};

typedef std::shared_ptr<TimeLinePlayback> TimeLinePlaybackPtr;