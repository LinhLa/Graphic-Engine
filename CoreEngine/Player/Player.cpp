#include "stdafx.h"
#include "Player.h"

#include "Scene.h"
#include "OriginProperty.h"
#include "LayoutProperty.h"

#include "PropertyDefine.h"

#include "UIHelper.h"
#include "SignalDefine.h"

#include "AnimationTimeLine.h"
#include "time_suffix.h"

#include "PlayerButton.h"
#include "BackGround.h"
#include "FullSizeSymbol.h"
#include "Speaker.h"
#include "ProgressBar.h"
#include "TimeSlide.h"

#include "KeyMap.h"
#include "KeyCommand.h"

#include "nfd.h"

using namespace UIHelper;

SDL_Rect toSDLRect(glm::vec3 transform, glm::vec2 size)
{
	SDL_Rect result = {
		static_cast<int>(transform.x),
		static_cast<int>(transform.y),
		static_cast<int>(size.x),
		static_cast<int>(size.y)
	};
	return result;
}

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
	m_pPlayer.lock()->SetPropertyValue(VISIBLE, visible);
}

EmptyNodePtr Player::getPlayer()
{
	return m_pPlayer.lock();
}

void Player::init(VoidType&& dummy)
{
	LOG_DEBUG("");
	addComponent(PlayerButton::create(shared_from_this()));
	addComponent(BackGround::create(shared_from_this()));
	addComponent(PlayerButton::create(shared_from_this()));
	addComponent(TimeSlide::create(shared_from_this()));
	addComponent(ProgressBar::create(shared_from_this()));
	addComponent(Speaker::create(shared_from_this()));
	addComponent(FullSizeSymbol::create(shared_from_this()));

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
}

void Player::PrepareAnimation()
{
	auto playButtonPtr = getComponent<PlayerButton>();
	if (playButtonPtr)
	{
		playButtonPtr->PlayButtonFadeIn();
		playButtonPtr->PlayButtonZoomIn();
	}
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
	auto pPlayerBackground = Scene::GetInstance()->LookupUIObject<Node2DImage>(PLAYER_BACKGROUND);
	auto pLanscape = Scene::GetInstance()->LookupUIObject<Node2DImage>(LAND_SCAPE);

	auto pPlayerNode = EmptyNode::create(VIDEO_PLAYER);
	Scene::GetInstance()->AddResource(pPlayerNode);
	Scene::GetInstance()->AddToSceneGraph(pPlayerNode);
	
	pPlayerNode->addChild(pLanscape);
	pPlayerNode->addChild(pPlayerBackground);
	/*pPlayerNode->SetPropertyValue<float>(ANGLE, 5.0f);
	pPlayerNode->SetPropertyValue<glm::vec3>(SCALE_VECTOR, glm::vec3(0.5f));
	pPlayerNode->SetPropertyValue<glm::vec3>(TRANS_VECTOR, glm::vec3(150.0f, 100.0f, 0.0f));
	pPlayerNode->SetPropertyValue<bool>(IS_CLIP, true);
	pPlayerNode->SetPropertyValue<float>(LAYOUT_WIDTH, 800.0f);
	pPlayerNode->SetPropertyValue<float>(LAYOUT_HEIGHT, 600.0f);*/

	m_pPlayer = pPlayerNode;
	for (auto &existedComponent : m_ComponentList)
	{
		existedComponent->AcquireResource();
	}
}

FocusPtr Player::getFocus()
{
	return m_pFocus;
}