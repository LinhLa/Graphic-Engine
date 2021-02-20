#pragma once
#include "creator.h"
#include "SingletonHolder.h"
#include <unordered_map>
#include <functional>
#include "Key.h"

class KeyMap final : public SingletonHolder<KeyMap>
{
private:
	std::unordered_map<uint32_t, KeyPtr> m_rootKey;
	KeyPtr 		m_CurrentKey = nullptr;
	bool		m_bLongKeyPress = false;

	KeyMap();

	void onKeyPress(uint32_t);
	void onKeyRelease(uint32_t);
public:
	~KeyMap();
	friend class SingletonHolder<KeyMap>;

	void init();
	void addKey(uint32_t, KeyPtr);

	uint32_t CurrentKeyCode();
	bool	IsLongKeyPress();

	void onKeyInput(uint32_t, uint32_t);
};