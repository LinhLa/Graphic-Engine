/*
Glossary:
BS byte:	block size byte - lie in LSB of memory block
Guard byte: lie in MSB of memory block

structure of free-list memory block:
[LSB ...block size.....|.........|.... guard byte ... MSB]
[LSB ....8bits.........|.........|.....8bits.... MSB]

structure of memory chunk block:
[LSB ...block size.....|.........|.... guard byte ... MSB]
[LSB ....8-bytes.......|.........|.....8bits.... MSB]


*/

#include "stdafx.h"
#include "memmgr.h"
#include "ThreadModel.h"
#include <exception>
#include <math.h>       /* ceil */

#define POOL_SIZE size_t(1024) ///<number of elements in a single pool can be chosen based on application requirements
#define CHUNK_SIZE size_t(1024*1024)
#define MAX_CHUNK_SIZE size_t(0xFF - 2)
#define GUARD_BYTE	0xde

static void InitialiseGuardByteList(void* base, void* end, const size_t block_size);
static size_t AssertGuardByte(void* object);

std::atomic_int64_t IMemoryManager::s_total = 0;
std::atomic_int64_t IMemoryManager::s_usage = 0;

template<size_t N>
FreeListObjectMemory<N>::FreeListObjectMemory()
{
	memset(m_baseList, sizeof(m_baseList), 0);
	m_currentIndex = 0;
}

template<size_t N>
FreeListObjectMemory<N>::~FreeListObjectMemory()
{
	for (uint32_t index = 0; index < m_currentIndex; ++index)
	{
		delete m_baseList[index];
	}
}

template<size_t N>
void FreeListObjectMemory<N>::Initialise()
{
	base = malloc(N * POOL_SIZE);
	endbase = &(static_cast<char*>(base)[N * POOL_SIZE]);

	//<Push new memory address to managed list
	m_baseList[m_currentIndex++] = reinterpret_cast<char*>(base);

	//<Setup guard byte to every block
	InitialiseGuardByteList(base, endbase, N);

	//<Move base pointer to 1st byte
	base = &static_cast<char*>(base)[1];

	s_total += N * POOL_SIZE;
}

template<size_t N>
void* FreeListObjectMemory<N>::allocate(const size_t& size)
{
	register void* blockPtr = nullptr;
	IMemoryManager::s_usage.fetch_add(N);
	{
		MultiThreadModel<FreeListObjectMemory> AutoLock(_0);
		if (m_freeList)
		{
			blockPtr = m_freeList;
			m_freeList = m_freeList->next;
		}
	}

	if (!blockPtr)
	{
		MultiThreadModel<FreeListObjectMemory> AutoLock(_1);
		if (!base || base > endbase)
		{
			Initialise();
		}
		blockPtr = static_cast<uint8_t*>(base) + 1;
		base = &(static_cast<uint8_t*>(base)[N]);
	}

	_ASSERT(blockPtr);
	printf("FreeListObjectMemory<%d>::%s block pointer[%8X]", N, __FUNCTION__, blockPtr);
	return blockPtr;
}

template<size_t N>
void  FreeListObjectMemory<N>::deallocate(void* object)
{
	IMemoryManager::s_usage.fetch_sub(N);
	MultiThreadModel<FreeListObjectMemory> AutoLock;
	printf("FreeListObjectMemory<%d>::%s object[%8X]", N, __FUNCTION__, object);
	if (!m_freeList)
	{
		m_freeList = static_cast<FreeStore*>(object);
		m_freeList->next = nullptr;
	}
	else
	{
		static_cast<FreeStore*>(object)->next = m_freeList;
		m_freeList = static_cast<FreeStore*>(object);
	}
}

FreeListMemoryManager::FreeListMemoryManager()
{
	Initialise();
}

FreeListMemoryManager::~FreeListMemoryManager()
{
}

void FreeListMemoryManager::Initialise()
{
	m_memory_list[0] = &obj_memory_16;
	m_memory_list[1] = &obj_memory_32;
	m_memory_list[2] = &obj_memory_48;
	m_memory_list[3] = &obj_memory_64;
	m_memory_list[4] = &obj_memory_96;
	m_memory_list[5] = &obj_memory_128;
	m_memory_list[6] = &obj_memory_256;

	memset(&m_array_driven[0], 0x00, 14 + 1);
	memset(&m_array_driven[15], 0x01, 30 - 14);
	memset(&m_array_driven[31], 0x02, 46 - 30);
	memset(&m_array_driven[47], 0x03, 62 - 46);
	memset(&m_array_driven[63], 0x04, 94 - 62);
	memset(&m_array_driven[95], 0x05, 126 - 94);
	memset(&m_array_driven[127], 0x06, 254 - 126);
}


void* FreeListMemoryManager::allocate(const size_t& size)
{
	_ASSERT(size <= MAX_FREE_LIST_SIZE);
	return m_memory_list[m_array_driven[size]]->allocate(size);
}

void  FreeListMemoryManager::deallocate(void* object)
{
	if (!object)
	{
		return;///<return on null pointer
	}
	register size_t actual_block_size = AssertGuardByte(object) - 2/*Block size byte and Guard byte*/;
	m_memory_list[m_array_driven[actual_block_size]]->deallocate(object);
}

size_t AssertGuardByte(void* object)
{
	register uint8_t* pointer = static_cast<uint8_t*>(object);
	/* memory block as below: */
	/*LSB (pointer - 1)-->[block size].....|pointer --> [here].....|(pointer-1 + block size - 1) --> [guard byte] ... MSB| */
	_ASSERT(GUARD_BYTE == (pointer - 1)[*(pointer - 1) - 1]);
	return *(pointer - 1);///<return block size
}

void InitialiseGuardByteList(void* base, void* end, const size_t block_size)
{
	register uint8_t* ubase = static_cast<uint8_t*>(base);
	while (ubase < end)
	{
		/*LSB ...block size.....|.........|.... guard byte ... MSB| */
		/*LSB ....8bits.........|.........|.....8bits.... MSB| */
		ubase[0] = (block_size & 0xFF);	  ubase[block_size - 1] = (GUARD_BYTE & 0xFF);
		ubase = &ubase[block_size];
	}
}

DynamicMemorySizeManager::DynamicMemorySizeManager()
{
	Initialise();
}

DynamicMemorySizeManager::~DynamicMemorySizeManager()
{
	for (uint32_t index = 0; index < m_currentIndex; ++index)
	{
		delete m_baseList[index];
	}
}


void DynamicMemorySizeManager::Initialise()
{
	//<chunk size is 1024 Mbytes
	base = malloc(CHUNK_SIZE);
	endbase = &(static_cast<char*>(base)[CHUNK_SIZE]);

	//<Push new memory address to managed list
	m_baseList[m_currentIndex++] = reinterpret_cast<char*>(base);

	s_total += CHUNK_SIZE;
}

size_t DynamicMemorySizeManager::AssertGuardByte(void* object)
{
	/* memory block as below: */
	/*LSB (pointer - 2-bytes)-->[block size].....|pointer[here].....|(pointer + block size) --> [guard byte] ... MSB| */
	register uint32_t* pBlockSize = static_cast<uint32_t*>(object) - 1;
	register uint8_t*  pointer = static_cast<uint8_t*>(object);
	_ASSERT(GUARD_BYTE == pointer[*pBlockSize]);
	return *pBlockSize;///<return block size
}

void* DynamicMemorySizeManager::allocate(const size_t& size)
{
	_ASSERT(size <= MAX_DYNAMIC_SIZE);
	register void* blockPtr = nullptr;
	register size_t _size = 2 /*Block size byte*/ + size + 1 /*Guard Byte*/;

	IMemoryManager::s_usage.fetch_add(_size);
	// Get the first block that is large enough to encompass Size bytes
	auto smallestBlockItr = m_FreeBlocksBySize.lower_bound(_size);
	if (smallestBlockItr != m_FreeBlocksBySize.end())
	{
		blockPtr = reinterpret_cast<void*>(&(smallestBlockItr->second->StartAddress));
	}

	if (!blockPtr)
	{
		MultiThreadModel<DynamicMemorySizeManager> AutoLock(_1);
		if (!base || base > endbase)
		{
			Initialise();
		}

		FreeBlockInfo* pBlockInfo = reinterpret_cast<FreeBlockInfo*>(base);
		uint8_t* ubase = static_cast<uint8_t*>(base);
		/*LSB ...block size.....|.........|.... guard byte ... MSB| */
		/*LSB ....16bits........|.........|.....8bits.... MSB| */
		pBlockInfo->Size = size;
		ubase[_size - 1] = (GUARD_BYTE & 0xFF);

		blockPtr = &(pBlockInfo->StartAddress);
		base = &(ubase[_size]);
	}

	_ASSERT(blockPtr);
	return blockPtr;
}

void  DynamicMemorySizeManager::deallocate(void* object)
{
	uint32_t blockSize = AssertGuardByte(object);

	IMemoryManager::s_usage.fetch_sub(2 /*Block size byte*/ + blockSize + 1 /*Guard Byte*/);
	uint8_t*  pointer = static_cast<uint8_t*>(object) - sizeof(BlockSizeType);
	m_FreeBlocksBySize.insert({blockSize, reinterpret_cast<FreeBlockInfo*>(pointer)});
}