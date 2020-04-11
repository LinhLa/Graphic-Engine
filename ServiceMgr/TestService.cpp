#include "stdafx.h"
#include "TestService.h"
#include <MyCustomDynamicAlloc.h>
#include <unordered_map>
#include <Timer.h>

static void gen_random_string(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

void TestDynamicSizeAllocator_001()
{
	//<test custom allocator
	std::unordered_map<
	size_t,
	size_t,
	MyHash<size_t, 1001>,
	std::equal_to<size_t>,
	MyCustomDynamicAlloc<std::pair<const size_t, size_t>>
	> hashTable;

	Timer timer;
	timer.start();

	for (long i = 0; i < 500000; ++i)
	{
		hashTable[i] = 100;
	}
	printf( "TestService::%s running time: [%d] (seconds) ", __FUNCTION__, timer.getTicks());
}

void TestDynamicSizeAllocator_002()
{
	//<test native STL allocator
	std::unordered_map<size_t, size_t> hashTable;

	Timer timer;
	timer.start();

	for (long i = 0; i < 500000; ++i)
	{
		hashTable[i] = 100;
	}
	printf( "TestService::%s running time: [%d] (seconds) ", __FUNCTION__, timer.getTicks());
}

void TestFreeMemoryList_001()
{
	//<test custom allocator
	std::list<int, MyCustomAlloc<int> > my_list;
	for (long i = 0; i < 500000; ++i)
	{
		my_list.push_back(i);
		my_list.pop_front();
	}
	return;
}

void TestFreeMemoryList_002()
{
	//<test native STL allocator
	std::list<int> my_list;
	for (long i = 0; i < 500000; ++i)
	{
		my_list.push_back(i);
		my_list.pop_front();
	}
	return;
}

TestService::TestService()
{
}

TestService::~TestService()
{
}

void TestService::onStartService()
{
	m_pServiceMgr->registerOnEvent(this, TEST_MEMORY);
	m_pServiceMgr->sendMessage({ TEST_MEMORY, 0ULL });
}

void TestService::onEndService()
{
	m_pServiceMgr->unregisterOnEvent(this, TEST_MEMORY);
}

void TestService::onMessageReceive(const Message &msg)
{
	switch (msg.m_id)
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
	//TestFreeMemoryList_001();
	//TestFreeMemoryList_002();
	TestDynamicSizeAllocator_001();
	TestDynamicSizeAllocator_002();
	//Test003();
}