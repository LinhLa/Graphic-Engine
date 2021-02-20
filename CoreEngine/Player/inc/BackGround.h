#pragma once
#include "IPlayerComponent.h"
#include "Player.h"
#include "Node2DImage.h"

class BackGround final : public IPlayerComponent, public creator<BackGround>
{
private:
	PlayerPtr 		m_pOwner = nullptr;
	Node2DImagePtr	m_pLanscape = nullptr;
	uint32_t		m_timerID = 0;

	void onPlayButtonChangeState(uint8_t&& state);
	void onPlay();
protected:
	BackGround(PlayerPtr);
public:
	virtual ~BackGround();
	friend class creator<BackGround>;

	void init() override;
	void AcquireResource() override;
	void onKeyInputEvent(SDL_Event&) override;

	void FocusIn() override;

	std::shared_ptr<Signal<int64_t>> pSignal_timeSlide = nullptr;
};
