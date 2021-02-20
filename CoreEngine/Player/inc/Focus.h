#pragma once
#include "IFocusComponent.h"
#include "creator.h"
#include <unordered_map>
#include "Key.h"

class FocusTable
{
protected:
	std::unordered_map<uint32_t, IFocusComponentPtr> m_componentList;
	typename std::unordered_map<uint32_t, IFocusComponentPtr>::iterator m_currentItr;
public:
	FocusTable();
	~FocusTable();

	void addComponent(IFocusComponentPtr);
	void removeComponent(uint32_t);
};

class Focus final: public creator<Focus>, public FocusTable
{
protected:
	Focus();
	KeyPtr m_pCurrentKeyPress = nullptr;
public:
	virtual ~Focus();
	friend class creator<Focus>;

	void onKeyInputEvent(SDL_Event& arg);
	void setFocus(uint32_t);
	void init();
};

typedef std::shared_ptr<Focus> FocusPtr;