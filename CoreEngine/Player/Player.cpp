#include "stdafx.h"
#include "Player.h"

#include "Scene.h"
#include "OriginProperty.h"
#include "LayoutProperty.h"

#include "PropertyDefine.h"
#include "macro_define.h"

#include "UIHelper.h"
#include "SignalDefine.h"

#include "AnimationTimeLine.h"
#include "time_suffix.h"

#include "PlayerButton.h"

#include "KeyMap.h"
#include "KeyCommand.h"

#include "nfd.h"

using namespace UIHelper;

Player::Player(){}
Player::~Player() {}

void Player::onKeyInputEvent(SDL_Event &&arg)
{
	//key map have hight priority
	KeyMap::GetInstance()->onKeyInput(arg.type, arg.key.keysym.scancode);

	//Only send key event to focus component
	m_pFocus->onKeyInputEvent(arg);
}

void Player::setVisible(bool visible)
{
	m_pPlayer->SetPropertyValue(VISIBLE, visible);
}

EmptyNodePtr Player::getPlayer()
{
	return m_pPlayer;
}

void Player::init(VoidType&& dummy)
{
	LOG_DEBUG("");
	//<Focus should be create before component init
	m_pFocus = Focus::create();

	for (auto &existedComponent : m_ComponentList)
	{
		existedComponent->init();
	}

	//<Focus should be init after all component
	m_pFocus->init();

	AcquireResource();
	PrepareKeyCommand();
	PrepareAnimation();
	PrepareKeyMap();

	Scene::GetInstance()->AddToSceneGraph(m_pPlayer);
}

void Player::PrepareAnimation()
{
	auto playButtonPtr = getComponent<PlayerButton>();
	playButtonPtr->PlayButtonFadeIn();
	playButtonPtr->PlayButtonZoomIn();
}

void Player::PrepareKeyCommand()
{
	std::function<void()> openFileDialog = []() {
		nfdchar_t *outpath = NULL;
		NFD_OpenDialog(NULL, NULL, &outpath);
		if (outpath)
		{
			free(outpath);
		}
	};
	KeyCommand::GetInstance()->addCommand("File Dialog", openFileDialog);
}

void Player::PrepareKeyMap()
{
	KeyPtr pLCtrlKey = Key::create(SDL_SCANCODE_LCTRL, "");
	pLCtrlKey->addKey({SDL_SCANCODE_O} , Key::create(SDL_SCANCODE_O, "File Dialog"));

	KeyMap::GetInstance()->addKey(pLCtrlKey->keyCode(), pLCtrlKey);
}

void Player::AcquireResource()
{
	LOG_DEBUG("");
	m_pPlayerBackground = Scene::GetInstance()->LookupUIObject<Node2DImage>(PLAYER_BACKGROUND);
	auto pLanscape = Scene::GetInstance()->LookupUIObject<Node2DImage>(LAND_SCAPE);
	m_pPlayer = EmptyNode::create(VIDEO_PLAYER);
	m_pPlayer->addChild(pLanscape);
	m_pPlayer->addChild(m_pPlayerBackground);
	for (auto &existedComponent : m_ComponentList)
	{
		existedComponent->AcquireResource();
	}
}

FocusPtr Player::getFocus()
{
	return m_pFocus;
}