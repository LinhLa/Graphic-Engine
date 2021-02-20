#ifndef __MEMMGR_H_
#define __MEMMGR_H_
/**
* @file memmgr.h
*
* @brief
*
* @ingroup Common.lib
* (Note: this needs exactly one @defgroup somewhere)
*
* @author Linh la
* Contact: lamanhlinh2000@yahoo.com
*
*/

#include <atomic>
#include <map>
#include "log.h"
#include "SingletonHolder.h"
#include "ThreadModel.h"


#define MAX_FREE_LIST_SIZE 			size_t(253)
#define MAX_DYNAMIC_SIZE 			size_t(1021)
#define MAX_BLOCK_MEMORY_MANAGED 	size_t(1024)


/**
 * @brief      This class describes a memory manager interface.
 */
class IMemoryManager
{
protected:
	struct FreeStore {
		FreeStore* next = nullptr;
	};

	static int64_t s_total;
	static int64_t s_usage;
public:
	static int64_t total() { return s_total; }
	static int64_t usage() { return s_usage; }
	static int64_t free() { return s_total - s_usage; }
	IMemoryManager() {}
	virtual ~IMemoryManager() {}

	virtual void* allocate(const size_t&) = 0;
	virtual void  deallocate(void*) = 0;
};


template<size_t N = 16>
class FreeListObjectMemory final :public IMemoryManager
{
private:
	friend class FreeListMemoryManager;
	char*  		m_baseList[MAX_BLOCK_MEMORY_MANAGED];
	uint32_t	m_currentIndex;

	void*		base = nullptr;
	void*		endbase = nullptr;
	FreeStore*	m_freeList = nullptr;

	FreeListObjectMemory();
	~FreeListObjectMemory();

	void Initialise();

public:
	void* allocate(const size_t& size) override;
	void   deallocate(void* object) override;
};

class FreeListMemoryManager final : public SingletonHolder<FreeListMemoryManager>
{
private:
	friend class SingletonHolder<FreeListMemoryManager>;

	FreeListObjectMemory<16> obj_memory_16;
	FreeListObjectMemory<32> obj_memory_32;
	FreeListObjectMemory<48> obj_memory_48;
	FreeListObjectMemory<64> obj_memory_64;
	FreeListObjectMemory<96> obj_memory_96;
	FreeListObjectMemory<128> obj_memory_128;
	FreeListObjectMemory<255> obj_memory_255;

	IMemoryManager* m_memory_list[7];
	uint8_t m_array_driven[MAX_FREE_LIST_SIZE + 1];
	FreeListMemoryManager();
	~FreeListMemoryManager();
	void Initialise();

public:
	void* allocate(const size_t& size);
	void  deallocate(void* object);
};

//Global overloading of new and delete operator
//void * operator new(size_t size);
//void operator delete(void * object);

#define FREE_LIST_MEMORY_MANAGER(CLASS_NAME)	\
void* operator new(size_t size)	\
{	\
	return FreeListMemoryManager::GetInstance()->allocate(sizeof(##CLASS_NAME));	\
}	\
void  operator delete(void* object)	\
{	\
	FreeListMemoryManager::GetInstance()->deallocate(object);	\
}	\

class DynamicMemorySizeManager final : public IMemoryManager, public SingletonHolder<DynamicMemorySizeManager>
{
protected:
	friend class SingletonHolder<DynamicMemorySizeManager>;

	typedef uint16_t BlockSizeType;
	struct FreeBlockInfo
	{
		// Block size (no reserved space for the size of allocation)
		BlockSizeType 	Size = 0;
		uint8_t 		StartAddress = 0;
		FreeBlockInfo(BlockSizeType _Size) : Size(_Size){}
	};

	// Type of the map that keeps memory blocks sorted by their sizes
	using FreeBlocksBySizeMap = std::multimap<BlockSizeType, FreeBlockInfo*>;
	char*  		m_baseList[MAX_BLOCK_MEMORY_MANAGED];
	uint32_t	m_currentIndex;

	void*		base = nullptr;
	void*		endbase = nullptr;

	FreeBlocksBySizeMap 	m_FreeBlocksBySize;

    DynamicMemorySizeManager();
    ~DynamicMemorySizeManager();

	void 	Initialise();
	size_t 	AssertGuardByte(void* object);
public:
	void* allocate(const size_t& size) override;
	void  deallocate(void* object) override;
 };

#define DYNAMIC_SIZE_MEMORY_MANAGER(CLASS_NAME)	\
void* operator new(size_t size)	\
{	\
	return DynamicMemorySizeManager ::GetInstance()->allocate(sizeof(##CLASS_NAME));	\
}	\
void  operator delete(void* object)	\
{	\
	DynamicMemorySizeManager::GetInstance()->deallocate(object);	\
}	\

#endif