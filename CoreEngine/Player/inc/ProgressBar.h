#pragma once
#include "IPlayerComponent.h"
#include "Player.h"
#include "Node2DImage.h"
#include "NodeText2D.h"

class ProgressBar final : public IPlayerComponent, public creator<ProgressBar>
{
private:
	PlayerPtr 		m_pOwner = nullptr;
	Node2DImagePtr	m_pSlider = nullptr;
	Node2DImagePtr	m_pElapsedTime = nullptr;
	Node2DImagePtr	m_pScrubbingBar = nullptr;

	void onScrubbingBarKeyPress(uint32_t&&, uint32_t&&);
	void updateSliderPosition(SDL_Point&&);
	void onSilerMove();

protected:
	ProgressBar(PlayerPtr);
public:
	virtual ~ProgressBar();
	friend class creator<ProgressBar>;

	void init() override;
	void AcquireResource() override;
	void onKeyInputEvent(SDL_Event&) override;

	void FocusIn() override;
};
