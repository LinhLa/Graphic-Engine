#include "stdafx.h"
#include <log.h>
#include "LogService.h"
#include "ServiceMgr.h"

LogService::LogService()
{

}


LogService::~LogService()
{

}

void LogService::onStartService()
{
	m_pServiceMgr->registerOnEvent(this, CONFIG_UPDATE_LOG_LEVEL);
	m_pServiceMgr->registerOnEvent(this, LOG_DATA_IN);
}

void LogService::onEndService()
{
	m_pServiceMgr->unregisterOnEvent(this, CONFIG_UPDATE_LOG_LEVEL);
	m_pServiceMgr->unregisterOnEvent(this, LOG_DATA_IN);
}
void LogService::onMessageReceive(const Message &msg)
{
	switch (msg.m_uValue)
	{
	case CONFIG_UPDATE_LOG_LEVEL:
		LogObject::SetLogLevel(TLogLevel(msg.m_uValue));
		break;
	case LOG_DATA_IN:
		//< TODO: Floot to log queue
		break;
	default:
		break;
	}
}