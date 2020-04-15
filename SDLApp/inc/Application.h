#pragma once
#include <iostream>
#include <SingletonHolder.h>
#include <ServiceMgr.h>
class Application : public SingletonHolder<Application>
{
private:
	friend class SingletonHolder<Application>;
	Application();
public:
	virtual ~Application();
	void start();
};