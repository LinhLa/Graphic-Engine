#pragma once
#include "IPlayerComponent.h"
#include "Player.h"
#include "Node2DImage.h"

class PlayerButton final: public IPlayerComponent, public creator<PlayerButton>
{
public:
	enum BUTTON_STATE : uint8_t
	{
		PAUSE = 0,
		PLAY = 1,
		TOTAL,
	};
private:
	uint8_t	m_currentState = PLAY;
	std::string		m_textureArray[TOTAL];

	PlayerPtr 		m_pOwner = nullptr;
	Node2DImagePtr	m_pPlaybuttonBackground = nullptr;
	Node2DImagePtr	m_PlayButton = nullptr;

	void changeState();
protected:
	PlayerButton(PlayerPtr);
public:
	virtual ~PlayerButton();
	friend class creator<PlayerButton>;

	void init() override;
	void AcquireResource() override;
	void onKeyInputEvent(SDL_Event&) override;

	void FocusIn() override;

	void PlayButtonFadeIn();
	void PlayButtonZoomIn();

	std::shared_ptr<Signal<uint8_t>> pSignal_state = nullptr;
	static const std::string STATE_SIGNAL;
};
