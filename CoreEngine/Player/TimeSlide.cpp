#include "stdafx.h"
#include "TimeSlide.h"
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

TimeSlide::TimeSlide(PlayerPtr pOwner)
{
	m_pOwner = pOwner;
}
TimeSlide::~TimeSlide() {}

void TimeSlide::init()
{
	m_pOwner->getFocus()->addComponent(shared_from_this());

	auto pBackGround = m_pOwner->getComponent<BackGround>();
	pBackGround->pSignal_timeSlide->bind(this, &TimeSlide::onTimeSlideChange);
}

void TimeSlide::AcquireResource()
{
	m_pHours = Scene::GetInstance()->LookupUIObject<NodeText2D>(HOURS);
	m_pMinutes = Scene::GetInstance()->LookupUIObject<NodeText2D>(MINUTES);
	m_pSeconds = Scene::GetInstance()->LookupUIObject<NodeText2D>(SECONDS);
	m_pMSeconds = Scene::GetInstance()->LookupUIObject<NodeText2D>(MSECONS);
	m_pColonArray[0] = Scene::GetInstance()->LookupUIObject<NodeText2D>("colon_h");
	m_pColonArray[1] = Scene::GetInstance()->LookupUIObject<NodeText2D>("colon_m");
	m_pColonArray[2] = Scene::GetInstance()->LookupUIObject<NodeText2D>("colon_s");
	auto pPlayer = m_pOwner->getPlayer();
	pPlayer->addChild(m_pHours);
	pPlayer->addChild(m_pMinutes);
	pPlayer->addChild(m_pSeconds);
	pPlayer->addChild(m_pMSeconds);
	pPlayer->addChild(m_pColonArray[0]);
	pPlayer->addChild(m_pColonArray[1]);
	pPlayer->addChild(m_pColonArray[2]);
}

void TimeSlide::onKeyInputEvent(SDL_Event& arg)
{}

void TimeSlide::FocusIn()
{}

void TimeSlide::onTimeSlideChange(int64_t&& time)
{
	//LOG_DEBUG("time[%ld]", time);
	char temp_str[3];
	temp_str[2] = 0;
	int64_t hh = time / (1000*60*60);
	int64_t mm = time / (1000*60) - hh*60;
	int64_t ss = time / (1000) - mm*60 - hh*3600;
	int64_t ms = time - ss*1000 - mm*60*1000 - hh*3600*1000;

	snprintf(temp_str, sizeof(temp_str), "%02lld", hh);
	m_pHours->SetPropertyValue<std::string>(TEXT, std::move(std::string(temp_str)));
	snprintf(temp_str, sizeof(temp_str), "%02lld", mm);
	m_pMinutes->SetPropertyValue<std::string>(TEXT, std::move(std::string(temp_str)));
	snprintf(temp_str, sizeof(temp_str), "%02lld", ss);
	m_pSeconds->SetPropertyValue<std::string>(TEXT, std::move(std::string(temp_str)));
	snprintf(temp_str, sizeof(temp_str), "%02lld", ms);
	m_pMSeconds->SetPropertyValue<std::string>(TEXT, std::move(std::string(temp_str)));
}