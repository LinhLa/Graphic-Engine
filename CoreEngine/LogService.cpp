#include "stdafx.h"
#include <log.h>
#include "LogService.h"
#include "ServiceMgr.h"

#include "base_define.h"
LogService::LogService()
{

}


LogService::~LogService()
{

}

void LogService::onStartService()
{
	LOG_DEBUG("LogService is started");
	m_pServiceMgr->registerEvent(this, CONFIG_UPDATE_LOG_LEVEL);
	m_pServiceMgr->registerEvent(this, LOG_DATA_IN);
}

void LogService::onEndService()
{
	m_pServiceMgr->unregisterEvent(this, CONFIG_UPDATE_LOG_LEVEL);
	m_pServiceMgr->unregisterEvent(this, LOG_DATA_IN);
}
void LogService::onMessageReceive(MessagePtr msg)
{
	switch (msg->m_uValue)
	{
	case CONFIG_UPDATE_LOG_LEVEL:
		LogObject::SetLogLevel(TLogLevel(msg->m_uValue));
		break;
	case LOG_DATA_IN:
		//< TODO: Floot to log queue
		break;
	default:
		break;
	}
}