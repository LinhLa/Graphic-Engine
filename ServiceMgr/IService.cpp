#include "stdafx.h"
#include <IService.h>
#include <ServiceMgr.h>

ServiceMgr* IService::m_pServiceMgr = ServiceMgr::GetInstance();
IService::IService()
{
	_ASSERT(m_pServiceMgr);
}

IService::~IService() {}