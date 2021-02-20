#pragma once
#include <list>
#include <memory>
#include "log.h"

/**
 * @brief      forward declairation
 */
class PropertyMethodTable;

/**
 * @brief      This class describes a interface of property method.
 */
class IPropertyMethod
{
public:
	IPropertyMethod(){}
	virtual ~IPropertyMethod(){}
	virtual void init() = 0;
};

typedef std::shared_ptr<IPropertyMethod> IPropertyMethodSharedPtr;

class PropertyMethodTable
{
protected:
	std::list<IPropertyMethodSharedPtr> m_propertyMethodTable;
public:
	PropertyMethodTable();
	virtual ~PropertyMethodTable();

	/**
	 * @brief      Gets the property method object.
	 *
	 * @tparam     T     { Concreate type}
	 *
	 * @return     The property method object.
	 */
	template<class T>
	std::shared_ptr<T> GetPropertyMethodObj()
	{
		static_assert(std::is_base_of<IPropertyMethod, T>::value, "T must derive from IPropertyMethod");
		for (auto& exisitingObj : m_propertyMethodTable)
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
	void AddPropertyMethodObj(std::shared_ptr<T> pMethodObject)
	{
		static_assert(std::is_base_of<IPropertyMethod, T>::value, "T must derive from IPropertyMethod");
		if (!pMethodObject)
		{
			LOG_DEBUG("Method Object[nullptr]");
			return;
		}

		for (auto& exisitingObj : m_propertyMethodTable)
		{
			if (std::dynamic_pointer_cast<T>(exisitingObj))
            {
               ///<do nothing because existed
				return;
            }
		}
		///<push new method object with init properties
		m_propertyMethodTable.push_back(pMethodObject);
		pMethodObject->init();
	}
};