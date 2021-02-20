#include "stdafx.h"
#include "LifeAndDeathBook.h"

LifeAndDeathBook::LifeAndDeathBook(){}
LifeAndDeathBook::~LifeAndDeathBook(){}
void LifeAndDeathBook::live(void* o)
{
	m_book[reinterpret_cast<uint64_t>(o)] = true;
}

void LifeAndDeathBook::die(void* o)
{
	m_book[reinterpret_cast<uint64_t>(o)] = false;
}

bool LifeAndDeathBook::isAlive(void* o)
{
	if (m_book.find(reinterpret_cast<uint64_t>(o)) != m_book.end())
	{
		return m_book[reinterpret_cast<uint64_t>(o)];
	}
	return false;
}

bool LifeAndDeathBook::isDeath(void* o)
{
	if (m_book.find(reinterpret_cast<uint64_t>(o)) != m_book.end())
	{
		return m_book[reinterpret_cast<uint64_t>(o)];
	}
	return true;
}

BookWritter::BookWritter()
{
	LifeAndDeathBook::GetInstance()->live(reinterpret_cast<void*>(this));
}

BookWritter::~BookWritter()
{
	LifeAndDeathBook::GetInstance()->die(reinterpret_cast<void*>(this));
}
