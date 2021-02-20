#include "stdafx.h"
#include "Key.h"
#include "KeyCommand.h"
#include <SDL.h>
#include "log.h"

Key::Key(uint32_t keyCode, std::string command) :m_keyCode(keyCode)
{
	m_keyReleaseCallback = KeyCommand::GetInstance()->getCommand(command);
}

Key::~Key() {}

void Key::addKey(KeyPtr pKey)
{
	if (pKey)
	{
		m_subKey[pKey->keyCode()] = pKey;
	}
}

void Key::addKey(std::list<uint32_t> KeyCodeList, KeyPtr pKey)
{
	if (1U == KeyCodeList.size())
	{
		m_subKey[KeyCodeList.front()] = pKey;
		return;
	}

	//search for next key
	KeyPtr pNextKey = nullptr;
	auto keyIterator = m_subKey.find(KeyCodeList.front());


	if (keyIterator != m_subKey.end())
	{
		//found next key
		pNextKey = keyIterator->second;
	}
	else
	{
		//not found then create new key
		pNextKey = Key::create(KeyCodeList.front(), "");

		//add to sub key list
		m_subKey[KeyCodeList.front()] = pNextKey;
	}

	//pop out key code
	KeyCodeList.pop_front();

	//recursive add key
	pNextKey->addKey(KeyCodeList, pKey);
}

void Key::removeKey(uint32_t keyCode)
{
	m_subKey.erase(keyCode);
}

void Key::onRelease()
{
	if (nullptr != m_keyReleaseCallback)
	{
		m_keyReleaseCallback();
	}
}

uint32_t Key::keyCode() const
{
	return m_keyCode;
}

KeyPtr Key::getNextKey(uint32_t keyCode)
{
	auto keyIterator = m_subKey.find(keyCode);
	if (keyIterator != m_subKey.end())
	{
		return keyIterator->second;
	}
	return nullptr;
}
