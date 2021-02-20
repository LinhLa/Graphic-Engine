#pragma once
#include "IPlayerComponent.h"
#include "Player.h"
#include "Node2DImage.h"
#include "NodeText2D.h"

class TimeSlide final : public IPlayerComponent, public creator<TimeSlide>
{
private:
	PlayerPtr 		m_pOwner = nullptr;
	NodeText2DPtr	m_pHours = nullptr;
	NodeText2DPtr	m_pMinutes = nullptr;
	NodeText2DPtr	m_pSeconds = nullptr;
	NodeText2DPtr	m_pMSeconds = nullptr;
	NodeText2DPtr	m_pColonArray[3];
	void onTimeSlideChange(int64_t&&);

protected:
	TimeSlide(PlayerPtr);
public:
	virtual ~TimeSlide();
	friend class creator<TimeSlide>;

	void init() override;
	void AcquireResource() override;
	void onKeyInputEvent(SDL_Event&) override;

	void FocusIn() override;
};
