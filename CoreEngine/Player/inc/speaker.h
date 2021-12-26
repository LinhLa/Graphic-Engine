#pragma once
#include "IPlayerComponent.h"
#include "Player.h"
#include "Node2DImage.h"
class Speaker final : public IPlayerComponent, public creator<Speaker>
{
private:
	std::weak_ptr<Player> 		m_pOwner;
	Node2DImagePtr	m_pSpeaker = nullptr;

protected:
	Speaker(PlayerPtr);
public:;
	virtual ~Speaker();
	friend class creator<Speaker>;

	void init() override;
	void AcquireResource() override;
	void onKeyInputEvent(SDL_Event&) override;

	void FocusIn() override;
};
