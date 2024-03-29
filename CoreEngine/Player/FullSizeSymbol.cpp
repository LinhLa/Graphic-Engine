#include "stdafx.h"
#include "FullSizeSymbol.h"
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

FullSizeSymbol::FullSizeSymbol(PlayerPtr pOwner)
{
	m_pOwner = pOwner;
}
FullSizeSymbol::~FullSizeSymbol() {}

void FullSizeSymbol::init()
{
	m_pOwner.lock()->getFocus()->addComponent(shared_from_this());
}

void FullSizeSymbol::AcquireResource()
{
	m_pFullSize = Scene::GetInstance()->LookupUIObject<Node2DImage>(FULL_SIZE_SYMBOL);
	auto pPlayer = m_pOwner.lock()->getPlayer();
	pPlayer->addChild(m_pFullSize);
}

void FullSizeSymbol::onKeyInputEvent(SDL_Event& arg)
{
	//LOG_DEBUG("");
}

void FullSizeSymbol::FocusIn()
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
	pTimeline->addTimelinePlayback(TimeLinePlayback::create(m_pFullSize, opacityAnimation));
}