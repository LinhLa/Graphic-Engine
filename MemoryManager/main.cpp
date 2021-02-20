#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <unordered_map>
#include "MyCustomDynamicAlloc.h"
#include "MyCustomAlloc.h"
#include "ObjectPool.h"
//#include "Timer.h"

namespace test
{
	namespace objectpool
	{
		struct dummy {};
		struct dummyPool: public ObjectPool<dummy>, SingletonHolder<dummyPool>
		{
		private:
			friend class SingletonHolder<dummyPool>;
			dummy * Create()
			{
				return new dummy;
			}
		protected:
			dummyPool(){}
			virtual ~dummyPool(){}
		};

		void TestObjectPool_001()
		{
			dummy *dummyArray[20];
			printf("Get object from pool\n");
			for (size_t i = 0; i < 20; ++i)
			{
				dummyArray[i] = dummyPool::GetInstance()->Get();
				printf("object number in pool[%d]\n", dummyPool::GetInstance()->size());
			}
			printf("push object to pool\n");
			for (auto o: dummyArray)
			{
				dummyPool::GetInstance()->Push(o);
				printf("object number in pool[%d]\n", dummyPool::GetInstance()->size());
			}
		}
	}

	namespace memory
	{
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

		//	Timer timer;
		//	timer.start();

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
			std::list<int, MyCustomDynamicAlloc<int> > my_list;

			//Timer timer;
			//timer.start();

			for (long i = 0; i < 500; ++i)
			{
				my_list.push_back(i);
				my_list.pop_front();
			}

			//LOG_DEBUG("running time: [%d] (seconds) ", timer.getTicks());
			return;
		}

		void TestDynamicSizeAllocator_002()
		{
			std::list<int, MyCustomDynamicAlloc<int> > my_list;
			//Timer timer;
			//timer.start();
			for (long i = 0; i < 500; ++i)
			{
				my_list.push_back(i);
			}
			//LOG_DEBUG("running time: [%d] (seconds) ", timer.getTicks());
			return;
		}

		void TestFreeMemoryList_001()
		{
			//<test custom allocator
			std::list<int, MyCustomAlloc<int> > my_list;
			for (long i = 0; i < 5000; ++i)
			{
				my_list.push_back(i);
				my_list.pop_front();
			}
			return;
		}

		void TestFreeMemoryList_002()
		{
			//<test custom allocator
			std::list<int, MyCustomAlloc<int> > my_list;
			for (long i = 0; i < 5000; ++i)
			{
				my_list.push_back(i);
			}
			return;
		}

	}
}

int main(int argc, char* argv[])
{
	//test::TestDynamicSizeAllocator_002();
	test::objectpool::TestObjectPool_001();
	char c;
	std::cin>>c;
	return 0;
}
