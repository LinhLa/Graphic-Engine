#pragma once
#include <functional>
#include <memory>
#include "creator.h"
enum class DATA_TYPE:uint8_t
{
	SCALA = 0,
	RAW_MEMORY,
	POINTER_TO_OBJECT,
};

struct Message : public creator<Message>
{
	friend class creator<Message>;

	uint64_t m_id = 0ULL;
	DATA_TYPE m_DataType = DATA_TYPE::SCALA;
	std::function<void()> m_destructor = nullptr;
	void *m_pointer = nullptr;
	union
	{
		uint64_t m_uValue;
		int64_t  m_iValue;
		float	 m_fValue;
		double	 m_dValue;
		bool	 m_bValue;
		uint16_t m_uDouble16byte[2];
		void*	 m_pointer;
	};

	Message() {}

	Message(const Message& other) {
		m_id = other.m_id;
		m_DataType = other.m_DataType;
		m_destructor = other.m_destructor;
		if (DATA_TYPE::RAW_MEMORY == m_DataType || DATA_TYPE::POINTER_TO_OBJECT == m_DataType)
		{
			m_pointer = other.m_pointer;
		}
		else
		{
			m_uValue = other.m_uValue;
		}
	}

	Message(uint64_t msg_id, uint64_t value) : m_id(msg_id), m_uValue(value), m_DataType(DATA_TYPE::SCALA) {}
	Message(uint64_t msg_id, int64_t value) :m_id(msg_id), m_iValue(value), m_DataType(DATA_TYPE::SCALA) {}
	Message(uint64_t msg_id, float value) :m_id(msg_id), m_fValue(value), m_DataType(DATA_TYPE::SCALA) {}
	Message(uint64_t msg_id, double value) :m_id(msg_id), m_dValue(value), m_DataType(DATA_TYPE::SCALA) {}
	Message(uint64_t msg_id, bool value) :m_id(msg_id), m_bValue(value), m_DataType(DATA_TYPE::SCALA) {}

	Message(uint64_t msg_id, void* pObject, std::function<void()> destructor = nullptr) :
	m_id(msg_id), m_pointer(pObject), m_DataType(DATA_TYPE::POINTER_TO_OBJECT), m_destructor(destructor)
	{}

	Message(uint64_t msg_id, void* pData, size_t len, std::function<void()> destructor = nullptr) :
	m_id(msg_id), m_DataType(DATA_TYPE::RAW_MEMORY), m_destructor(destructor)
	{
		_ASSERT(pData && len >= 0);
		m_pointer = malloc(len);
		memcpy(m_pointer, pData, len);
	}

	~Message()
	{
		if (DATA_TYPE::RAW_MEMORY == m_DataType && m_pointer)
		{
			free(m_pointer);
			m_pointer = nullptr;
		}

		if (DATA_TYPE::POINTER_TO_OBJECT == m_DataType && m_pointer)
		{
			//call destructor
			m_destructor();
			m_pointer = nullptr;
		}
	}

	bool operator<(const Message& other)
	{
		return (m_id < other.m_id);
	}

	bool operator==(const Message& other)
	{
		return (m_id == other.m_id);
	}
};

typedef std::shared_ptr<Message> MessagePtr;