#include "stdafx.h"
#include <iostream>
#include "MyCustomDynamicAlloc.h"
#include <unordered_map>
//
//void TestDynamicSizeAllocator_001()
//{
//	//<test custom allocator
//	std::unordered_map<
//		size_t,
//		size_t,
//		MyHash<size_t, 1001>,
//		std::equal_to<size_t>,
//		MyCustomDynamicAlloc<std::pair<const size_t, size_t>>
//	> hashTable;
//
//	Timer timer;
//	timer.start();
//
//	for (long i = 0; i < 500000; ++i)
//	{
//		hashTable[i] = 100;
//	}
//	printf("TestService::%s running time: [%d] (seconds) ", __FUNCTION__, timer.getTicks());
//}

void TestDynamicSizeAllocator_002()
{
	//<test native STL allocator
	std::unordered_map<size_t, size_t> hashTable;

	//Timer timer;
	//timer.start();

	for (long i = 0; i < 500000; ++i)
	{
		hashTable[i] = 100;
	}
	//printf("TestService::%s running time: [%d] (seconds) ", __FUNCTION__, timer.getTicks());
}


int main(int argc, char* argv[])
{
	TestDynamicSizeAllocator_002();
	return 0;
}
