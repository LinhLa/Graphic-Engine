#include "stdafx.h"
#include "Focus.h"
#include "KeyMap.h"
#include "UIHelper.h"

#include "AnimationTimeLine.h"
#include "time_suffix.h"
using namespace UIHelper;

FocusTable::FocusTable()
{
	m_currentItr = m_componentList.end();
}

FocusTable::~FocusTable() {}

void FocusTable::addComponent(IFocusComponentPtr pComponent)
{
	if (pComponent)
	{
		m_componentList[pComponent->getFocusIndex()] = pComponent;
	}
}

void FocusTable::removeComponent(uint32_t index)
{
	auto itrResult = m_componentList.find(index);
	if (itrResult != m_componentList.end())
	{
		if (m_currentItr == itrResult)
		{
			m_currentItr = m_componentList.erase(itrResult);
		}
		else
		{
			m_componentList.erase(itrResult);
		}
	}
}

Focus::Focus() {}
Focus::~Focus() {}

void Focus::onKeyInputEvent(SDL_Event& arg)
{
	if (m_componentList.empty())
	{
		return;
	}

	const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (SDL_KEYDOWN == arg.type && currentKeyStates[SDL_SCANCODE_TAB])
	{
		if (++m_currentItr == m_componentList.end())
		{
			m_currentItr = m_componentList.begin();
		}
		m_currentItr->second->FocusIn();
	}
	else
	{
		//Broadcast mouse event
		switch (arg.type)
		{
		case SDL_MOUSEMOTION:
		case SDL_MOUSEWHEEL:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			for (auto &existedComponent : m_componentList)
			{
				existedComponent.second->onKeyInputEvent(arg);
			}
			break;
		case SDL_KEYDOWN:
			if (!KeyMap::GetInstance()->IsLongKeyPress())
			{
				m_currentItr->second->onKeyInputEvent(arg);
			}
			break;
		case SDL_KEYUP:
		case SDL_TEXTINPUT:
			m_currentItr->second->onKeyInputEvent(arg);
			break;
		default:
			break;
		}
	}
}

void Focus::setFocus(uint32_t index)
{
	auto itrResult = m_componentList.find(index);
	if (itrResult != m_componentList.end())
	{
		m_currentItr = itrResult;
	}
}

void Focus::init()
{
	m_currentItr = m_componentList.begin();
}
