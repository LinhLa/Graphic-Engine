#pragma once
#include "component.h"
#include <SDL.h>
#include "IFocusComponent.h"
class IPlayerComponent: public IFocusComponent, public std::enable_shared_from_this<IPlayerComponent>
{
public:
	virtual void init() = 0;
	virtual void AcquireResource() = 0;
};

typedef std::shared_ptr<IPlayerComponent> IPlayerComponentPtr;
typedef std::shared_ptr<IFocusComponent> IFocusComponentPtr;