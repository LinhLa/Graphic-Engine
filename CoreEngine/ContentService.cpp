#include "stdafx.h"
#include <ContentService.h>
#include <ServiceMgr.h>
#include <Scene.h>
#include "macro_define.h"
#include "PropertyDefine.h"
ContentService::ContentService()
{}

ContentService::~ContentService()
{
	//<Remove trigger event to service manager
}

void ContentService::onStartService()
{
	LOG_DEBUG("ContentService is started");
}

void ContentService::onEndService()
{
}

void ContentService::onMessageReceive(MessagePtr msg)
{
	switch (msg->m_id)
	{
	default:
		break;
	}
}