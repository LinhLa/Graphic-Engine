#pragma once
#include <argument_define.h>

class function_base
{
public:
	function_base() {}
	virtual ~function_base() {}

	void operator()(ArgumentBase *arg)
	{
		return raise(arg);
	}

	virtual void raise(ArgumentBase*) = 0;
};

template<typename CLASS, typename ArgumentType>
class function_member : public function_base
{
private:
	typedef void(CLASS::*METHOD)(ArgumentType &);
	CLASS *m_object;
	METHOD m_method;
public:
	function_member(CLASS *object, METHOD method) :m_object(object), m_method(method) {}
	function_member(const function_member &other)
	{
		m_object = other.m_object;
		m_method = other.m_method;
	}

	void raise(ArgumentBase *arg) override
	{
		return (m_object->*m_method)(*dynamic_cast<ArgumentType*>(arg));
	}

	function_member& operator=(const function_member &other)
	{
		m_object = other.m_object;
		m_method = other.m_method;
	}

	bool operator==(const function_member& other)
	{
		if (this == &other)
		{
			return true;
		}

		if (other.m_object == m_object && other.m_method == m_method)
		{
			return true;
		}

		return false;
	}

	FREE_LIST_MEMORY_MANAGER(function_member)
};

template<typename ArgumentType>
class function_global : public function_base
{
private:
	typedef void(*METHOD)(ArgumentType&);
	METHOD m_method;
public:
	function_global(METHOD method) :m_method(method) {}
	function_global(const function_global &other)
	{
		m_method = other.m_method;
	}

	void raise(ArgumentBase* arg) override
	{
		return (*m_method)(*dynamic_cast<ArgumentType*>(arg));
	}

	function_global& operator=(const function_global &other)
	{
		m_method = other.m_method;
	}

	bool operator==(const function_global& other)
	{
		if (this == &other || other.m_method == m_method)
		{
			return true;
		}

		return false;
	}

	FREE_LIST_MEMORY_MANAGER(function_global)
};