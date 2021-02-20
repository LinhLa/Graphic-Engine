#pragma once
#include "SingletonHolder.h"
#include <unordered_map>

class LifeAndDeathBook: public SingletonHolder<LifeAndDeathBook>
{
private:
	friend class SingletonHolder<LifeAndDeathBook>;
	LifeAndDeathBook();
	~LifeAndDeathBook();
	std::unordered_map<uint64_t, bool> m_book;
public:
	void live(void*);
	void die(void*);

	bool isAlive(void*);
	bool isDeath(void*);
};

class BookWritter
{
public:
	BookWritter();
	~BookWritter();
};