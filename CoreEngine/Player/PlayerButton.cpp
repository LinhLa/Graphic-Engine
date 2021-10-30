#include "stdafx.h"
#include "PlayerButton.h"
#include "Focus.h"

#include "Scene.h"
#include "OriginProperty.h"
#include "LayoutProperty.h"

#include "PropertyDefine.h"
#include "macro_define.h"

#include "UIHelper.h"
#include "SignalDefine.h"

#include "AnimationTimeLine.h"
#include "time_suffix.h"
using namespace UIHelper;
static const std::string STR_PLAY_ARROW = "play_arrow.png";
static const std::string STR_PLAY_PAUSE = "play_pause.png";
static const std::string STR_PLAY_STOP = "play_stop.png";

PlayerButton::PlayerButton(PlayerPtr pOwner)
{
	//<Assign owner
	m_pOwner = pOwner;
}

PlayerButton::~PlayerButton() {}

void PlayerButton::init()
{
	m_pOwner->getFocus()->addComponent(shared_from_this());

	m_textureArray[PLAY] = STR_PLAY_PAUSE;
	m_textureArray[PAUSE] = STR_PLAY_ARROW;

	m_currentState = PAUSE;

	pSignal_state = Signal<uint8_t>::create();
}

void PlayerButton::AcquireResource()
{
	m_pPlaybuttonBackground = Scene::GetInstance()->LookupUIObject<Node2DImage>(PLAY_BUTTON_BACKGROUND);
	m_PlayButton = Scene::GetInstance()->LookupUIObject<Node2DImage>(PLAY_BUTTON);
	m_PlayButton->SetTexture(m_textureArray[m_currentState]);
	auto pPlayer = m_pOwner->getPlayer();
	pPlayer->addChild(m_pPlaybuttonBackground);
	pPlayer->addChild(m_PlayButton);
}

void PlayerButton::FocusIn()
{
	PlayButtonFadeIn();
	PlayButtonZoomIn();
}

void PlayerButton::PlayButtonFadeIn()
{
	IKeyFramePtr keyframeList = AnimationKeyFrame<uint8_t>::create();
	keyframeList->addSplineKeyframe<uint8_t>(0_ms, 500_ms, 0, 255);

	AnimationPropertyPtr opacityAnimation = AnimationProperty::create();
	opacityAnimation->addEntry(OPACITY, keyframeList);

	AnimationTimeLine* pTimeline = AnimationTimeLine::GetInstance();
	pTimeline->addTimelinePlayback(TimeLinePlayback::create(m_pPlaybuttonBackground, opacityAnimation));
	pTimeline->addTimelinePlayback(TimeLinePlayback::create(m_PlayButton, opacityAnimation));
}

void PlayerButton::PlayButtonZoomIn()
{
	IKeyFramePtr keyframeList = AnimationKeyFrame<float>::create();
	keyframeList->addSplineKeyframe<float>(0_ms, 500_ms, 0.8F, 1.0F);

	AnimationPropertyPtr zoomAnimation = AnimationProperty::create();
	zoomAnimation->addEntry(SCALE_X, keyframeList);
	zoomAnimation->addEntry(SCALE_Y, keyframeList);

	AnimationTimeLine* pTimeline = AnimationTimeLine::GetInstance();
	pTimeline->addTimelinePlayback(TimeLinePlayback::create(m_pPlaybuttonBackground, zoomAnimation));
	pTimeline->addTimelinePlayback(TimeLinePlayback::create(m_PlayButton, zoomAnimation));
}

void PlayerButton::onKeyInputEvent(SDL_Event& arg)
{
	//LOG_DEBUG("");
	if (SDL_MOUSEBUTTONDOWN == arg.type)
	{
		auto layoutMethod = m_PlayButton->GetPropertyMethodObj<LayoutProperty>();
		SDL_Rect display_rect = layoutMethod->GetLayoutInformation();
		UIHelper::MOUSE_STATE state = UIHelper::onMouseEvent(arg, display_rect);
		switch (state)
		{
		case MOUSE_STATE::MOUSE_DOWN:
			changeState();
			break;
		default:
			break;
		}
	}
	else if (SDL_KEYDOWN == arg.type)
	{
		const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_RETURN])
		{
			changeState();
		}
	}
}

void PlayerButton::changeState()
{
	if (++m_currentState >= TOTAL)
	{
		m_currentState = PAUSE;
	}

	//<Update texture
	m_PlayButton->SetTexture(m_textureArray[m_currentState]);

	//<On signal
	pSignal_state->onSignal(std::move(uint8_t{ m_currentState }));
}