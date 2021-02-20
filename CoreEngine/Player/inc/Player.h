#pragma once
#include "IPlayerComponent.h"
#include "Focus.h"
#include "Node2DImage.h"
#include "NodeText2D.h"
#include "EmptyNode.h"
class Player final: public creator<Player>, public std::enable_shared_from_this<Player>, public Component<IPlayerComponent>
{
private:
	const std::string VIDEO_PLAYER = "VIDEO_PLAYER";
	const std::string FADE_IN_PROGRESS = "FADE_IN_PROGRESS";

	FocusPtr			m_pFocus = nullptr;
	EmptyNodePtr		m_pPlayer = nullptr;
	Node2DImagePtr		m_pPlayerBackground = nullptr;

	void AcquireResource();
	void PrepareAnimation();
	void PrepareKeyCommand();
	void PrepareKeyMap();
protected:
	Player();
public:
	~Player();
	friend class creator<Player>;

	void setVisible(bool visible);

	EmptyNodePtr	getPlayer();

	void init(VoidType&&);
	void onKeyInputEvent(SDL_Event&& arg);

	FocusPtr getFocus();
};
typedef std::shared_ptr<Player> PlayerPtr;