#pragma once
#include <list>
#include <queue_protected.h>
#include <unordered_map>
#include <map>
#include <string>
#include "Task.h"
#include "IService.h"
#include <MyCustomAlloc.h>
typedef queue_protected<Task*, MyCustomAlloc<Task*>> listTaskType;
typedef std::list<IService*, MyCustomAlloc<IService*>> listServiceType;
typedef queue_protected<Message, MyCustomAlloc<Message>> messageQueueType;

template <class KEY, size_t MOD = 101>
class MyHash {
public:
	size_t operator()(const KEY &key) const
	{
		return (key % MOD);
	}
};

template <size_t MOD>
class MyHash<std::string, MOD> {
public:
	size_t operator()(const std::string &key) const
	{
		size_t hash = 0;
		size_t base = 31;
		for (auto &character : key)
		{
			hash = (hash * base + character) % MOD;
		}
		return hash;
	}
};

class function_base;
typedef std::list<function_base*, MyCustomAlloc<function_base*>> listCallbackType;

typedef uint64_t message_id;
typedef std::map
<
	message_id,
	listCallbackType,
	std::less<message_id>,
	MyCustomAlloc<std::pair<const message_id, listCallbackType>>
>	dictionaryCallbackType;


struct configuration
{
	char version[8];
	char data_file[256];
	char asset_path[256];

	uint64_t pool_size;
	uint64_t chunk_size;

	uint8_t log_level;

	bool    full_screen;
	int32_t height;
	int32_t width;

	uint8_t fps;
	configuration():pool_size(0), chunk_size(0), log_level(0), full_screen(false), height(-1), width(-1), fps(30)
	{
		memset(version, 0, sizeof(version));
		memset(data_file, 0, sizeof(data_file));
		memset(asset_path, 0, sizeof(asset_path));
	}

	~configuration(){}
};
