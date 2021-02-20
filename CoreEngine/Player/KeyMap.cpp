#include "stdafx.h"
#include "KeyMap.h"
#include <SDL.h>
#include "log.h"

KeyMap::KeyMap(){}

KeyMap::~KeyMap() {}

void KeyMap::addKey(uint32_t keyCode, KeyPtr pKey)
{
	m_rootKey[keyCode] = pKey;
}

void KeyMap::init()
{}

void KeyMap::onKeyPress(uint32_t keyCode)
{
	if (nullptr == m_CurrentKey)
	{
		auto keyIterator = m_rootKey.find(keyCode);
		if (keyIterator != m_rootKey.end())
		{
			m_CurrentKey = keyIterator->second;
		}
	}
	else if (m_CurrentKey->keyCode() != keyCode)
	{
		m_CurrentKey = m_CurrentKey->getNextKey(keyCode);
		m_bLongKeyPress = false;
	}
	else if (m_CurrentKey->keyCode() == keyCode)
	{
		m_bLongKeyPress = true;
		LOG_DEBUG("Key hold");
	}
	else
	{
		LOG_DEBUG("Shouldn't be here");
	}
}

void KeyMap::onKeyRelease(uint32_t keyCode)
{
	if (nullptr == m_CurrentKey)
	{
		return;
	}

	if (m_CurrentKey->keyCode() == keyCode)
	{
		m_CurrentKey->onRelease();
	}
	else
	{
		LOG_DEBUG("Key release mismatch");
	}
	m_CurrentKey = nullptr;
}

void KeyMap::onKeyInput(uint32_t keyEvent, uint32_t keyCode)
{
	if (m_rootKey.empty())
	{
		return;
	}

	switch (keyEvent)
	{
	case SDL_KEYUP:
		onKeyRelease(keyCode);
		break;
	case SDL_KEYDOWN:
		onKeyPress(keyCode);
		break;
	default:
		break;
	}
}

uint32_t KeyMap::CurrentKeyCode()
{
	if (nullptr == m_CurrentKey)
	{
		return 0;
	}
	return m_CurrentKey->keyCode();
}

bool KeyMap::IsLongKeyPress()
{
	return m_bLongKeyPress;
}
