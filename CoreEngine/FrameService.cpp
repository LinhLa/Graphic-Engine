#include "stdafx.h"
#include "log.h"
#include "ServiceMgr.h"
#include "FrameService.h"

FrameService::FrameService()
{
	//<Add trigger event to service manager

}
FrameService::~FrameService()
{
	//<Remove trigger event to service manager

}

void FrameService::FrameReport()
{

}

void FrameService::onMessageReceive(MessagePtr msg)
{
}

void FrameService::onStartService()
{
	LOG_DEBUG("FrameService is started");
}


void FrameService::onEndService()
{
}
