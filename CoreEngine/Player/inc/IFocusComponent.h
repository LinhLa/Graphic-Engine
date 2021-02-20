#pragma once
#include <cstdint>
#include <memory>
#include <SDL.h>
class IFocusComponent
{
protected:
	static int32_t s_count;
	int32_t m_iFocusIndex = 0;
public:
	IFocusComponent();
	~IFocusComponent();
	int32_t getFocusIndex() const;
	virtual void FocusIn() = 0;
	virtual	void onKeyInputEvent(SDL_Event&) = 0;
};

typedef std::shared_ptr<IFocusComponent> IFocusComponentPtr;