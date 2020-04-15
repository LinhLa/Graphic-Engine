#pragma once
#include <IMessageHandler.h>
#include <SceneService.h>

enum {
	STAGE_1 = 0,
	STAGE_2,
	STAGE_3,
	STAGE_4,
	STAGE_5,
	STAGE_MAX,
};

class IArea
{
protected:
	IArea();
public:
	virtual ~IArea();
};

typedef std::shared_ptr<IArea> IAreaPtr;