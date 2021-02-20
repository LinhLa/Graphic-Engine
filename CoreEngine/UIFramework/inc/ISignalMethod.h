#pragma once
#include <list>
#include <memory>
#include "log.h"

/**
 * @brief      forward declairation
 */
class SignalMethodTable;

/**
 * @brief      This class describes a interface of property method.
 */
class ISignalMethod
{
public:
	ISignalMethod(){}
	virtual ~ISignalMethod(){}
	virtual void init() = 0;
};

typedef std::shared_ptr<ISignalMethod> ISignalMethodSharedPtr;

class SignalMethodTable
{
protected:
	std::list<ISignalMethodSharedPtr> m_signalMethodTable;
public:
	SignalMethodTable();
	virtual ~SignalMethodTable();

	/**
	 * @brief      Gets the property method object.
	 *
	 * @tparam     T     { Concreate type}
	 *
	 * @return     The property method object.
	 */
	template<class T>
	std::shared_ptr<T> GetSignalMethodObj()
	{
		static_assert(std::is_base_of<ISignalMethod, T>::value, "T must derive from ISignalMethod");
		for (auto& exisitingObj : m_signalMethodTable)
		{
			if (std::dynamic_pointer_cast<T>(exisitingObj))
            {
                return std::dynamic_pointer_cast<T>(exisitingObj);
            }
        }
        return nullptr;
	}

	/**
	 * @brief      Adds a property method object and init all property
	 *
	 * @param[in]  pMethodObject  The method object
	 *
	 * @tparam     T              { concreate type }
	 */
	template<class T>
	void AddSignalMethodObj(std::shared_ptr<T> pMethodObject)
	{
		static_assert(std::is_base_of<ISignalMethod, T>::value, "T must derive from ISignalMethod");
		if (!pMethodObject)
		{
			LOG_DEBUG("Method Object[nullptr]");
			return;
		}

		for (auto& exisitingObj : m_signalMethodTable)
		{
			if (std::dynamic_pointer_cast<T>(exisitingObj))
            {
                ///<do nothing because existed
				return;
            }
		}
		///<push new method object with init properties
		m_signalMethodTable.push_back(pMethodObject);
		pMethodObject->init();
	}
};