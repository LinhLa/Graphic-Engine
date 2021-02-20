#include "stdafx.h"
#include "BackGround.h"
#include "PlayerButton.h"

#include "Scene.h"
#include "OriginProperty.h"
#include "LayoutProperty.h"

#include "PropertyDefine.h"
#include "macro_define.h"

#include "UIHelper.h"
#include "time_suffix.h"

#include "AnimationTimeLine.h"
#include "TaskPoolInstance.h"
using namespace UIHelper;

static int64_t elapsed_time = 0;
static uint8_t direction = 0;
BackGround::BackGround(PlayerPtr pOwner)
{
	m_pOwner = pOwner;
}
BackGround::~BackGround() {}

void BackGround::init()
{
	pSignal_timeSlide = Signal<int64_t>::create();
	auto pPlayerButton = m_pOwner->getComponent<PlayerButton>();
	pPlayerButton->pSignal_state->bind(this, &BackGround::onPlayButtonChangeState);
}

void BackGround::onPlay()
{
	if (0 == elapsed_time)
	{
		return;
	}

	if (0 > elapsed_time)
	{
		elapsed_time = 0;
		//<On signal time slide
		pSignal_timeSlide->onSignal(std::move(int64_t{ 0 }));
		return;
	}

	auto pLayout = m_pLanscape->GetPropertyMethodObj<LayoutProperty>();
	SDL_Rect rect = pLayout->GetLayoutInformation();
	if (0 == direction)
	{
		if (rect.x - 3 > -663)
		{
			pLayout->SetLayoutPosition(rect.x - 3, rect.y);
		}
		else
		{
			direction = 1;
		}
	}
	else if (1 == direction )
	{
		if (rect.x + 3 < 0)
		{
			pLayout->SetLayoutPosition(rect.x + 3, rect.y);
		}
		else
		{
			direction = 0;
		}
	}
	//<On signal time slide
	pSignal_timeSlide->onSignal(std::move(int64_t{ elapsed_time -= 16 }));
}

void BackGround::AcquireResource()
{
	m_pLanscape = Scene::GetInstance()->LookupUIObject<Node2DImage>(LAND_SCAPE);
}

void BackGround::onKeyInputEvent(SDL_Event& arg)
{
	//LOG_DEBUG("");
}

void BackGround::FocusIn() {}

void BackGround::onPlayButtonChangeState(uint8_t&& state)
{
	switch (state)
	{
	case PlayerButton::PLAY:
		m_timerID = TaskPoolInstance::GetInstance()->addRepeateTask(RepetitionTask::create(16, std::bind(&BackGround::onPlay, this)));
		elapsed_time = 10000;
		direction = 0;
		break;
	case PlayerButton::PAUSE:
		TaskPoolInstance::GetInstance()->removeTask(m_timerID);
		break;
	default:
		break;
	}
}