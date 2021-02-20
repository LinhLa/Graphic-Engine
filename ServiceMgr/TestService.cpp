#include "stdafx.h"
#include "TestService.h"
#include "base_define.h"
TestService::TestService()
{
}

TestService::~TestService()
{
}

void TestService::onStartService()
{
	m_pServiceMgr->registerEvent(this, TEST_MEMORY);
}

void TestService::onEndService()
{
	m_pServiceMgr->unregisterEvent(this, TEST_MEMORY);
}

void TestService::onMessageReceive(MessagePtr msg)
{
	switch (msg->m_id)
	{
	case TEST_MEMORY:
		process();
		break;
	default:
		break;
	}
}

void TestService::process()
{
	printf("TestService::[%s]", __FUNCTION__);
}