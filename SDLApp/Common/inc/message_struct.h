#pragma once
struct message_data
{
	message_data(void* pData, size_t len) :m_len(len), m_pData(pData)
	{
		_ASSERT(m_pData);
	}

	~message_data() {}

	size_t m_len = 0;
	void *m_pData = nullptr;

	FREE_LIST_MEMORY_MANAGER(message_data)
};

struct message_resource
{
	message_resource(void** pObject, int32_t type, const char* resource_name) :m_pObject(pObject), m_type(type), m_thread_id(std::this_thread::get_id())
	{
		//<pObject is querried so it is always null
		_ASSERT(resource_name && (strlen(resource_name) < sizeof(m_pResourceName)));

		memset(m_pResourceName, 0, sizeof(m_pResourceName));
		strncpy(m_pResourceName, resource_name, strlen(resource_name));
	}

	message_resource(const message_resource& other) :m_pObject(other.m_pObject), m_type(other.m_type), m_thread_id(other.m_thread_id)
	{
		_ASSERT(m_pObject);
		memset(m_pResourceName, 0, sizeof(m_pResourceName));
		strncpy(m_pResourceName, other.m_pResourceName, sizeof(m_pResourceName));
	}

	~message_resource() {}
	int32_t				m_type = -1;
	void**				m_pObject = nullptr;
	char				m_pResourceName[32];
	std::thread::id		m_thread_id;
	FREE_LIST_MEMORY_MANAGER(message_resource)
};

struct message_function
{
	message_function(function_base* pFunct, int32_t eventType) :m_pFunct(pFunct), m_evenType(eventType)
	{
		_ASSERT(pFunct);
	}

	~message_function()
	{
	}

	int32_t		m_evenType = -1;
	function_base *m_pFunct = nullptr;

	FREE_LIST_MEMORY_MANAGER(message_function)

};

struct Message {

	Message() {
		memset(m_message_data, 0, sizeof(m_message_data));
	}

	Message(const Message& other) {
		m_id = other.m_id;
		memcpy(m_message_data, other.m_message_data, sizeof(m_message_data));
	}

	Message(uint64_t msg_id, uint64_t value) : m_id(msg_id), m_uValue(value) { }
	Message(uint64_t msg_id, int64_t value) :m_id(msg_id), m_iValue(value) {}
	Message(uint64_t msg_id, float value) :m_id(msg_id), m_fValue(value) {}
	Message(uint64_t msg_id, double value) :m_id(msg_id), m_dValue(value) {}
	Message(uint64_t msg_id, bool value) :m_id(msg_id), m_bValue(value) {}
	Message(uint64_t msg_id, message_data d) :m_id(msg_id), m_msg_data(d) {}
	Message(uint64_t msg_id, message_resource o) :m_id(msg_id), m_msg_object(o) {}
	Message(uint64_t msg_id, message_function f) :m_id(msg_id), m_msg_function(f) {}
	Message(uint64_t msg_id, const SDL_Event& event) :m_id(msg_id), m_event(event) {}
	Message(uint64_t msg_id, void* pData, size_t len) :m_id(msg_id)
	{
		_ASSERT(pData && len <= sizeof(m_message_data));
		memcpy(m_message_data, pData, len);
	}
	~Message() {}

	uint64_t m_id = 0ULL;
	union
	{
		uint64_t m_uValue;
		int64_t  m_iValue;
		float	 m_fValue;
		double	 m_dValue;
		bool	 m_bValue;
		uint16_t m_uDouble16byte[2];
		struct message_data		m_msg_data;
		struct message_resource	m_msg_object;
		struct message_function m_msg_function;
		SDL_Event		m_event;

		uint8_t			m_message_data[sizeof(SDL_Event)];
	};

	bool operator<(const Message& other)
	{
		return (m_id < other.m_id);
	}

	bool operator==(const Message& other)
	{
		return (m_id == other.m_id);
	}

	FREE_LIST_MEMORY_MANAGER(Message)
};

#define SIZE_OF_STRUCT_MESSAGE sizeof(Message);
