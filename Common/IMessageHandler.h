#pragma once
#include <type_define.h>
#include <base_define.h>

template<int32_t MIN_EVENT_ID = 0, int32_t MAX_EVENT_ID = 1>
class IMessageHandler
{
protected:
	listCallbackType	m_callback_list[MAX_EVENT_ID + 1];

	IMessageHandler(){}

public:

	virtual ~IMessageHandler(){}

	template<typename EVENT_STRUCT>
	void onEvent(EVENT_STRUCT &arg)
	{
		_ASSERT(EVENT_STRUCT::EventType >= MIN_EVENT_ID && EVENT_STRUCT::EventType <= MAX_EVENT_ID);
		for (auto &FunctionPointerType : m_callback_list[EVENT_STRUCT::EventType])
		{
			(*FunctionPointerType)(dynamic_cast<ArgumentBase*>(&arg));
		}
	}

	/** Adds a callback method to an event
	@param o instance of a class that contains the method to call
	@param method method to call*/
	template<typename CLASS, typename EVENT_STRUCT>
	void addMessageHandler(CLASS *o, void (CLASS::*method)(EVENT_STRUCT &arg))
	{
		_ASSERT(0 != method && 0 != o && EVENT_STRUCT::EventType >= MIN_EVENT_ID && EVENT_STRUCT::EventType <= MAX_EVENT_ID);
		//<Push callback without check for existing
		using FunctionType = function_member<CLASS, EVENT_STRUCT>;
		m_callback_list[EVENT_STRUCT::EventType].push_back(new FunctionType(o, method));
	}

	/** Adds a callback method to an event
	@param method method to call*/
	template<typename EVENT_STRUCT>
	void addMessageHandler(void(*method)(EVENT_STRUCT &arg))
	{
		_ASSERT(0 != method && EVENT_STRUCT::EventType >= MIN_EVENT_ID && EVENT_STRUCT::EventType <= MAX_EVENT_ID);
		//<Push callback without check for existing
		using FunctionType = function_global<EVENT_STRUCT>;
		m_callback_list[EVENT_STRUCT::EventType].push_back(new FunctionType(method));

	}

	/** Remove a callback method to an event
	@param o instance of a class that contains the method to call
	@param method method to call*/
	template<typename CLASS, typename EVENT_STRUCT>
	void removeMessageHandler(CLASS *o, void (CLASS::*method)(EVENT_STRUCT &arg))
	{
		_ASSERT(EVENT_STRUCT::EventType >= MIN_EVENT_ID && EVENT_STRUCT::EventType <= MAX_EVENT_ID);
		using FunctionType = function_member<CLASS, EVENT_STRUCT>;
		FunctionType f_mem(o, method);
		for (auto _itr = m_callback_list[EVENT_STRUCT::EventType].begin(); _itr != m_callback_list[EVENT_STRUCT::EventType].end(); ++_itr)
		{
			if (f_mem == *static_cast<FunctionType*>(*_itr))
			{
				m_callback_list[EVENT_STRUCT::EventType].erase(_itr);
			}
		}
	}

	/** Remove a callback method to an event
	@param method method to call*/
	template<typename EVENT_STRUCT>
	void removeMessageHandler(void(*method)(EVENT_STRUCT &arg))
	{
		_ASSERT(EVENT_STRUCT::EventType >= MIN_EVENT_ID && EVENT_STRUCT::EventType <= MAX_EVENT_ID);
		using FunctionType = function_global<EVENT_STRUCT>;
		FunctionType f_global(method);
		for (auto _itr = m_callback_list[EVENT_STRUCT::EventType].begin(); _itr != m_callback_list[EVENT_STRUCT::EventType].end(); ++_itr)
		{
			if (f_global == *static_cast<FunctionType*>(*_itr))
			{
				m_callback_list[EVENT_STRUCT::EventType].erase(_itr);
			}
		}
	}
};

