#include "stdafx.h"
#include "ProgressBar.h"
#include "BackGround.h"

#include "Focus.h"

#include "Scene.h"
#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "TextProperty.h"

#include "PropertyDefine.h"
#include "macro_define.h"

#include "UIHelper.h"
#include "SignalDefine.h"

#include "AnimationTimeLine.h"
#include "time_suffix.h"
using namespace UIHelper;

ProgressBar::ProgressBar(PlayerPtr pOwner)
{
	m_pOwner = pOwner;
}
ProgressBar::~ProgressBar() {}

void ProgressBar::init()
{
	m_pOwner.lock()->getFocus()->addComponent(shared_from_this());
}

void ProgressBar::AcquireResource()
{
	m_pElapsedTime = Scene::GetInstance()->LookupUIObject<Node2DImage>(ELAPSED_TIME);
	m_pScrubbingBar = Scene::GetInstance()->LookupUIObject<Node2DImage>(SCRUBBING_BAR);
	m_pSlider = Scene::GetInstance()->LookupUIObject<Node2DImage>(SLIDER);

	auto pPlayer = m_pOwner.lock()->getPlayer();
	pPlayer->addChild(m_pScrubbingBar);
	pPlayer->addChild(m_pElapsedTime);
	pPlayer->addChild(m_pSlider);

	m_pSlider->bind(MOUSE_DRAG_SIGNAL, this, &ProgressBar::updateSliderPosition);

	m_pScrubbingBar->bind(MOUSE_DOWN_SIGNAL, this, &ProgressBar::updateSliderPosition);
	m_pScrubbingBar->bind(KEY_PRESS_SIGNAL, this, &ProgressBar::onScrubbingBarKeyPress);

#ifdef OPENGL_RENDERING
	std::function<void(glm::vec3&&)> callback = [&](glm::vec3&& transform) {this->onSilerMove(); };
	m_pSlider->BindPropertySignal(TRANS_VECTOR, callback);
#else
	std::function<void(int&&)> callback = [&](int&& x) {this->onSilerMove(); };
	m_pSlider->BindPropertySignal(X_COORDINATOR, callback);
#endif
	
}

void ProgressBar::onKeyInputEvent(SDL_Event& arg)
{
	//LOG_DEBUG("");
	m_pSlider->onKeyInputEvent(arg);
	m_pScrubbingBar->onKeyInputEvent(arg);
	m_pElapsedTime->onKeyInputEvent(arg);
}

void ProgressBar::onScrubbingBarKeyPress(uint32_t&& keyState, uint32_t&& keyCode)
{
	auto sliderLayout = m_pSlider->GetPropertyMethodObj<LayoutProperty>();
	auto scrubBarLayout = m_pScrubbingBar->GetPropertyMethodObj<LayoutProperty>();
#ifdef OPENGL_RENDERING
	auto transform = sliderLayout->GetLayoutTransform();
	auto size = sliderLayout->GetLayoutSize();
	SDL_Rect slider_rect = toSDLRect(transform, size);

	transform = scrubBarLayout->GetLayoutTransform();
	size = scrubBarLayout->GetLayoutSize();
	SDL_Rect srubbar_rect = toSDLRect(transform, size);
#else
	SDL_Rect slider_rect = sliderLayout->GetLayoutInformation();
	SDL_Rect srubbar_rect = scrubBarLayout->GetLayoutInformation();
#endif
	if (SDL_KEYDOWN == keyState)
	{
		if (SDLK_LEFT == keyCode)
		{
			slider_rect.x -= 3;
		}
		if (SDLK_RIGHT == keyCode)
		{
			slider_rect.x += 3;
		}
	}
	if (slider_rect.x < (srubbar_rect.x - slider_rect.w / 2))
	{
		slider_rect.x = srubbar_rect.x - slider_rect.w / 2;
	}
	else if (slider_rect.x > (srubbar_rect.x + srubbar_rect.w - slider_rect.w / 2))
	{
		slider_rect.x = srubbar_rect.x + srubbar_rect.w - slider_rect.w / 2;
	}
	else
	{
	}
#ifdef OPENGL_RENDERING
	transform = sliderLayout->GetLayoutTransform();
	transform.x = slider_rect.x;
	transform.y = slider_rect.y;
	sliderLayout->SetLayoutTransform(transform);
#else
	sliderLayout->SetLayoutPosition(slider_rect.x, slider_rect.y);
#endif
}

void ProgressBar::updateSliderPosition(SDL_Point&& coordinates)
{
	auto sliderLayout = m_pSlider->GetPropertyMethodObj<LayoutProperty>();
	auto scrubBarLayout = m_pScrubbingBar->GetPropertyMethodObj<LayoutProperty>();
#ifdef OPENGL_RENDERING
	auto transform = sliderLayout->GetLayoutTransform();
	auto size = sliderLayout->GetLayoutSize();
	SDL_Rect slider_rect = toSDLRect(transform, size);

	transform = scrubBarLayout->GetLayoutTransform();
	size = scrubBarLayout->GetLayoutSize();
	SDL_Rect srubbar_rect = toSDLRect(transform, size);
#else
	SDL_Rect slider_rect = sliderLayout->GetLayoutInformation();
	SDL_Rect srubbar_rect = scrubBarLayout->GetLayoutInformation();
#endif
	int x = 0;
	if (coordinates.x < srubbar_rect.x)
	{
		x = srubbar_rect.x - slider_rect.w / 2;
	}
	else if (coordinates.x > (srubbar_rect.x + srubbar_rect.w))
	{
		x = srubbar_rect.x + srubbar_rect.w - slider_rect.w / 2;
	}
	else
	{
		x = coordinates.x - slider_rect.w / 2;
	}

#ifdef OPENGL_RENDERING
	transform = sliderLayout->GetLayoutTransform();
	transform.x = slider_rect.x;
	transform.y = slider_rect.y;
	sliderLayout->SetLayoutTransform(transform);
#else
	sliderLayout->SetLayoutPosition(slider_rect.x, slider_rect.y);
#endif
}

void ProgressBar::onSilerMove()
{
	auto sliderLayout = m_pSlider->GetPropertyMethodObj<LayoutProperty>();
	auto elapsedLayout = m_pElapsedTime->GetPropertyMethodObj<LayoutProperty>();
#ifdef OPENGL_RENDERING
	auto transform = sliderLayout->GetLayoutTransform();
	auto size = sliderLayout->GetLayoutSize();
	SDL_Rect slider_rect = toSDLRect(transform, size);

	transform = elapsedLayout->GetLayoutTransform();
	size = elapsedLayout->GetLayoutSize();
	SDL_Rect elapsed_rect = toSDLRect(transform, size);
#else
	SDL_Rect slider_rect = sliderLayout->GetLayoutInformation();
	SDL_Rect elapsed_rect = elapsedLayout->GetLayoutInformation();
#endif

	elapsed_rect.w = (slider_rect.x + slider_rect.w / 2) - elapsed_rect.x;

#ifdef OPENGL_RENDERING
	transform = elapsedLayout->GetLayoutTransform();
	transform.x = elapsed_rect.x;
	transform.y = elapsed_rect.y;
	elapsedLayout->SetLayoutTransform(transform);
#else
	elapsedLayout->SetLayoutInformation(elapsed_rect);
#endif	
}

void ProgressBar::FocusIn()
{
#ifdef OPENGL_RENDERING
	IKeyFramePtr keyframeList = AnimationKeyFrame<float>::create();
	keyframeList->addSplineKeyframe<float>(0_ms, 500_ms, 0.0f, 1.0f);
#else
	IKeyFramePtr keyframeList = AnimationKeyFrame<uint8_t>::create();
	keyframeList->addSplineKeyframe<uint8_t>(0_ms, 500_ms, 0, 255);
#endif
	
	AnimationPropertyPtr opacityAnimation = AnimationProperty::create();
	opacityAnimation->addEntry(OPACITY, keyframeList);

	AnimationTimeLine* pTimeline = AnimationTimeLine::GetInstance();
	pTimeline->addTimelinePlayback(TimeLinePlayback::create(m_pSlider, opacityAnimation));
}