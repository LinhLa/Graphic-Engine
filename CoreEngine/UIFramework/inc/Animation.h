#pragma once
#include <SDL.h>
#include <memory>
#include "creator.h"
class SpriteSheetAnimation: public creator<SpriteSheetAnimation>
{
private:
	uint32_t			m_CurrentFrame = 0;
	int32_t				m_FrameIncrement = 1;
	uint32_t			m_FrameRate = 0; //Milliseconds
	uint32_t			m_OldTime = 0;
	uint32_t			m_MaxFrames = 0;
	bool				m_Oscillate = false;
protected:
	SpriteSheetAnimation();
public:
	~SpriteSheetAnimation();
	friend class creator<SpriteSheetAnimation>;

	void Init(uint32_t framerate, uint32_t maxframe, bool oscillate);

	virtual void onAnimate() = 0;
	void reset();

	void SetFrameRate(uint32_t Rate);
	uint32_t GetFrameRate() const;

	void SetCurrentFrame(uint32_t Frame);
	uint32_t  GetCurrentFrame() const;

	void SetMaxFrames(uint32_t Frame);
	uint32_t  GetMaxFrames() const;

	void SetOscillate(bool osc);
	bool  GetOscillate() const;
};

typedef std::shared_ptr<SpriteSheetAnimation> AnimationPtr;