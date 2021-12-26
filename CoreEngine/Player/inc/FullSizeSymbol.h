#pragma once
#include "IPlayerComponent.h"
#include "Player.h"
#include "Node2DImage.h"
class FullSizeSymbol final : public IPlayerComponent, public creator<FullSizeSymbol>
{
private:
	std::weak_ptr<Player> 		m_pOwner;
	Node2DImagePtr	m_pSpeaker = nullptr;
	Node2DImagePtr	m_pFullSize = nullptr;

protected:
	FullSizeSymbol(PlayerPtr);
public:;
	virtual ~FullSizeSymbol();
	friend class creator<FullSizeSymbol>;

	void init() override;
	void AcquireResource() override;
	void onKeyInputEvent(SDL_Event&) override;

	void FocusIn() override;
};
