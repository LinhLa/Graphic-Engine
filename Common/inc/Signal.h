#pragma once
#include <memory>
#include <map>
#include <functional>
#include <utility>
#include <stdexcept>
#include "creator.h"
#include "LifeAndDeathBook.h"
#include "log.h"

/**
 * @brief      { Void Type for template parameter }
 */
struct VoidType
{
public:
	VoidType();
	~VoidType();
};
/**
 * @brief      forward declairation of Signal<> class
 *
 * @tparam     Args  { description }
 */
template<typename... Args> class Signal;


/**
 * @brief      This class describes a signal interface.
 */
class ISignal
{
public:
	ISignal();
	virtual ~ISignal();
	virtual const std::type_info& getType() = 0;

	template<typename... Args>
	void onSignal(Args&&... args)
	{
		if (typeid(std::function<void(Args&&...)>).hash_code() != getType().hash_code())
		{
			throw std::logic_error("invalid arguments");
		}
		dynamic_cast<Signal<Args...>*>(this)->onEvent(std::forward<Args>(args)...);
	}
};

typedef std::shared_ptr<ISignal> ISignalPtr;

/**
 * @brief      This class describes a signal.
 *
 * @tparam     Args  { description }
 */
template<typename... Args>
class Signal final: public ISignal, public creator<Signal<Args...>>
{
public:
	typedef std::function<void(Args&&...)> callableType;
	typedef std::map<uint64_t, callableType> listCallbackType;
protected:
	listCallbackType	m_callback_list;
	uint64_t			m_slot = 0;

	/**
	* @brief      Constructs a new instance.
	*/
	Signal() {}
public:

	/**
	 * @brief      Destroys the object.
	 */
	virtual ~Signal(){}
	friend class creator<Signal<Args...>>;

	/**
	 * @brief      Called on event.
	 *
	 * @param[in]  args  The arguments
	 */
	void onEvent(Args&&... args)
	{
		for (auto &Function : m_callback_list)
		{
			Function.second(std::forward<Args>(args)...);
		}
	}

	/**
	 * @brief      Adds a message handler.
	 *
	 * @param      o          { parameter_description }
	 * @param[in]  <unnamed>  { parameter_description }
	 *
	 * @tparam     CLASS      { description }
	 *
	 * @return     { description_of_the_return_value }
	 */
	template<typename CLASS>
	uint64_t bind(CLASS *o, void (CLASS::*method)(Args&&...))
	{
		m_callback_list[++m_slot] = [=](Args&&... args)->void
		{
			#ifdef ENABLE_LIFE_AND_DEATH_BOOK
			if (LifeAndDeathBook::GetInstance()->isAlive(static_cast<void*>(o)))
			{
				(o->*method)(std::forward<Args>(args)...);
			}else
			{
				throw std::logic_error("invoked call from deleted pointer");
			}
			#else
				(o->*method)(std::forward<Args>(args)...);
			#endif
		};
		return m_slot;
	}

	/**
	 * @brief      Adds a message handler.
	 *
	 * @param[in]  method  The method
	 *
	 * @return     { description_of_the_return_value }
	 */
	uint64_t bind(void(*method)(Args&&...))
	{
		m_callback_list[++m_slot] = [=](Args&&... args)->void{*method(std::forward<Args>(args)...);};
		return m_slot;
	}


	/**
	 * @brief      { function_description }
	 *
	 * @param[in]  funct  The funct
	 *
	 * @return     { description_of_the_return_value }
	 */
	uint64_t bind(callableType funct)
	{
		m_callback_list[++m_slot] = funct;
		return m_slot;
	}

	/**
	 * @brief      { function_description }
	 *
	 * @param      funct  The funct
	 *
	 * @tparam     U      { description }
	 *
	 * @return     { description_of_the_return_value }
	 */
	template<typename U>
	uint64_t bind(U&& funct)
	{
		throw std::logic_error("invalid arguments");
		return 0;
	}
	/**
	 * @brief      Removes a message handler.
	 *
	 * @param[in]  slot  The slot
	 */
	void unbind(uint64_t slot)
	{
		m_callback_list.erase(slot);
	}

	const std::type_info& getType() override
	{
		return typeid(callableType);
	}
};

/**
 * @brief      This class describes a signal table.
 */
class SignalTable
{
protected:
	std::unordered_map<std::string, ISignalPtr> m_signalTable;
	SignalTable();
public:
	~SignalTable();

	bool IsSignalExist(std::string);

	template<class T>
	bool AddSignal(std::string signal_name, std::shared_ptr<T> pSignal)
	{
		bool bResult = false;
		if (IsSignalExist(signal_name))
		{
			LOG_ERROR("Signal[%s] is exist", signal_name.c_str());
		}
		else if (std::is_base_of<ISignal, T>::value)
		{
			m_signalTable[signal_name] = std::dynamic_pointer_cast<ISignal>(pSignal);
			bResult = true;
		}
		return bResult;
	}

	void RemoveSignal(std::string);

	template<typename... Args>
	void OnSignal(std::string signal_name, Args&&... args)
	{
		auto itr = m_signalTable.find(signal_name);
		if (itr != m_signalTable.end())
		{
			itr->second->onSignal(std::forward<Args>(args)...);
		}
		else
		{
			LOG_ERROR("singal is not found [%s]", signal_name.c_str());
		}
	}

	template<typename... Args>
	uint64_t bind(std::string signal_name, std::function<void(Args&&...)> callback)
	{
		auto itr = m_signalTable.find(signal_name);
		if (itr != m_signalTable.end())
		{
			if (!std::dynamic_pointer_cast<Signal<Args...>>(itr->second))
			{
				throw std::logic_error("invalid arguments");
			}
			return std::dynamic_pointer_cast<Signal<Args...>>(itr->second)->bind(callback);
		}
		else
		{
			LOG_ERROR("signal is not found [%s]", signal_name.c_str());
		}
		return 0;
	}

	template<typename CLASS, typename... Args>
	uint64_t bind(std::string signal_name, CLASS *o, void (CLASS::*method)(Args&&...))
	{
		auto itr = m_signalTable.find(signal_name);
		if (itr != m_signalTable.end())
		{
			if (!std::dynamic_pointer_cast<Signal<Args...>>(itr->second))
			{
				throw std::logic_error("invalid arguments");
			}
			return std::dynamic_pointer_cast<Signal<Args...>>(itr->second)->bind(std::forward<CLASS*>(o), method);
		}
		else
		{
			LOG_ERROR("signal is not found [%s]", signal_name.c_str());
		}
		return 0;
	}

	template<typename... Args>
	uint64_t bind(std::string signal_name, void(*method)(Args&&...))
	{
		auto itr = m_signalTable.find(signal_name);
		if (itr != m_signalTable.end())
		{
			if (!std::dynamic_pointer_cast<Signal<Args...>>(itr->second))
			{
				throw std::logic_error("invalid arguments");
			}
			return std::dynamic_pointer_cast<Signal<Args...>>(itr->second)->bind(method);
		}
		else
		{
			LOG_ERROR("signal is not found [%s]", signal_name.c_str());
		}
		return 0;
	}

	template<typename... Args>
	void unbind(std::string signal_name, uint64_t slot)
	{
		auto itr = m_signalTable.find(signal_name);
		if (itr != m_signalTable.end())
		{
			if (!std::dynamic_pointer_cast<Signal<Args...>>(itr->second))
			{
				throw std::logic_error("invalid arguments");
			}
			std::dynamic_pointer_cast<Signal<Args...>>(itr->second)->unbind(slot);
		}
		else
		{
			LOG_ERROR("signal is not found [%s]", signal_name.c_str());
		}
	}
};