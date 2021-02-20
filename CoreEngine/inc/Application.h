#pragma once
#include <iostream>
#include <SingletonHolder.h>
#include "Signal.h"
class Application final: public SingletonHolder<Application>, public SignalTable
{
protected:
	friend class SingletonHolder<Application>;
	Application();


	void InitSignal();
	void InitService();

public:
	virtual ~Application();
	void start();
	void init();

	static const std::string POST_LOAD_RESOURCE_SIGNAL;
	static const std::string PRE_RENDER_SIGNAL;
	static const std::string POST_RENDER_SIGNAL;
	static const std::string KEY_INPUT_SIGNAL;
	static const std::string WINDOW_EVENT_SIGNAL;
};