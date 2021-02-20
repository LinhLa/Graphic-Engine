#include "stdafx.h"
#include "Speaker.h"
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

Speaker::Speaker(PlayerPtr pOwner)
{
	m_pOwner = pOwner;
}
Speaker::~Speaker() {}

void Speaker::init()
{
	m_pOwner->getFocus()->addComponent(shared_from_this());
}

void Speaker::AcquireResource()
{
	m_pSpeaker = Scene::GetInstance()->LookupUIObject<Node2DImage>(SPEAKER_SYMBOL);
	auto pPlayer = m_pOwner->getPlayer();
	pPlayer->addChild(m_pSpeaker);
}

void Speaker::onKeyInputEvent(SDL_Event& arg)
{
	//LOG_DEBUG("");
}


void Speaker::FocusIn()
{
	IKeyFramePtr keyframeList = AnimationKeyFrame<uint8_t>::create();
	keyframeList->addSplineKeyframe<uint8_t>(0_ms, 500_ms, 0, 255);

	AnimationPropertyPtr opacityAnimation = AnimationProperty::create();
	opacityAnimation->addEntry(OPACITY, keyframeList);

	AnimationTimeLine* pTimeline = AnimationTimeLine::GetInstance();
	pTimeline->addTimelinePlayback(TimeLinePlayback::create(m_pSpeaker, opacityAnimation));
}