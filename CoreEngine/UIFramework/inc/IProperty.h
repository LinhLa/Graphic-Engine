#pragma once
#include <exception>
#include <unordered_map>
#include <string.h>
#include <typeinfo>
#include <memory>
#include <sstream>
#include "creator.h"
#include "Signal.h"
#include "log.h"
#include <SDL.h>
template<class T>
T ConvertToType(std::string& strVal)
{
	std::stringstream sstream(strVal);
	T value;
	sstream >> value;
	return value;
}

enum PROPERTY_TYPE: uint8_t
{
	INT = 0,
	FLOAT = 1,
	BOOL = 3,
	COLOR = 4,
};

template<uint8_t>
struct PropertyType{};

template<>
struct PropertyType<0>
{
	typedef int Type;
};

template<>
struct PropertyType<1>
{
	typedef float Type;
};

template<>
struct PropertyType<2>
{
	typedef bool Type;
};

template<>
struct PropertyType<3>
{
	typedef SDL_Color Type;
};

/**
 * @brief      forward declairation
 */
template<class T> class Property;

/**
 * @brief      This class describes a property interface.
 */
class IProperty: public std::enable_shared_from_this<IProperty>
{
public:
	IProperty(){}
	virtual ~IProperty(){}
	virtual std::string getPropertyName() const = 0;
	virtual const std::type_info& getTypeInfo() const = 0;

private:
	template<typename U>
	Property<U>* DynamicTypeCheck()
	{
		if (typeid(U).hash_code() != getTypeInfo().hash_code())
		{
			throw std::logic_error("invalid_argument");
		}
		return dynamic_cast<Property<U>*>(this);
	}

	template<typename U>
	Property<U> const* DynamicTypeCheck() const
	{
		if (typeid(U).hash_code() != getTypeInfo().hash_code())
		{
			throw std::logic_error("invalid_argument");
		}
		return dynamic_cast<Property<U> const *>(this);
	}

public:
	template<typename U>
	void SetValue(U&& value)
	{
		DynamicTypeCheck<U>()->SetValue(std::forward<U>(value));
	}

	template<typename U>
	U GetValue() const
	{
		return DynamicTypeCheck<U>()->GetValue();
	}

	template<typename U, class V>
	void Visitor(V& visitor)
	{
		DynamicTypeCheck<U>()->Visitor(visitor);
	}

	template<typename U>
	uint64_t Bind(std::function<void(U&&)> callback)
	{
		return DynamicTypeCheck<U>()->m_pSignalValueChange->bind(callback);
	}

	template<typename U>
	void UnBind(uint64_t slot)
	{
		DynamicTypeCheck<U>()->m_pSignalValueChange->unbind(slot);
	}
};

typedef std::shared_ptr<IProperty> IPropertyPtr;
/**
 * @brief      This class describes a property.
 *
 * @tparam     T     { description }
 */
template<class T>
class Property final: public IProperty, public creator<Property<T>>
{
private:
	std::string 	m_property_name;
	T 				m_value;
public:
	std::shared_ptr<Signal<T>> 	m_pSignalValueChange;
protected:
	Property(std::string name, T&& value = T{}) :m_property_name(name), m_value(value)
	{
		m_pSignalValueChange = Signal<T>::create();
	}
public:
	virtual ~Property(){}

	friend class creator<Property<T>>;

	std::string getPropertyName() const override
	{
		return m_property_name;
	}

	const std::type_info& getTypeInfo() const override
	{
		return typeid(T);
	}

	void SetValue(T&& value)
	{
		m_value = value;
		m_pSignalValueChange->onEvent(std::forward<T>(value));
	}

	T GetValue() const
	{
		return m_value;
	}

	template<class T>
	void Visitor(T& visitor)
	{
		visitor.accept(this);
	}
};

/**
 * @brief      This class describes a property table.
 */
class PropertyTable
{
protected:
	std::unordered_map<std::string, IPropertyPtr> m_propertyTable;
	PropertyTable();
public:
	virtual ~PropertyTable();

	bool IsPropertyExist(std::string);

	bool AddProperty(std::string, IPropertyPtr);
	void RemoveProperty(std::string);

	template<typename U>
	void SetPropertyValue(std::string property_name, U value)
	{
		auto itr = m_propertyTable.find(property_name);
		if (itr != m_propertyTable.end())
		{
			itr->second->SetValue<U>(std::move(value));
		}
		else
		{
			//property name is not found, create new
			auto pProperty = Property<U>::create(property_name);

			//set value
			pProperty->SetValue(std::move(value));

			//add to table
			AddProperty(property_name, pProperty);
		}
	}

	template<typename U>
	U GetPropertyValue(std::string property_name) const
	{
		auto itr = m_propertyTable.find(property_name);
		if (itr != m_propertyTable.end())
		{
			return itr->second->GetValue<U>();
		}
		else
		{
			LOG_ERROR("property is not found: [%s]", property_name.c_str());
			_ASSERT(false);
		}
		return U{};
	}

	template<typename U, class V>
	void Visitor(std::string property_name, V& visitor)
	{
		auto itr = m_propertyTable.find(property_name);
		if (itr != m_propertyTable.end())
		{
			itr->second->Visitor<U,V>(std::forward<V>(visitor));
		}
		else
		{
			LOG_ERROR("property name is not found: [%s]", property_name.c_str());
			_ASSERT(false);
		}
	}

	template<typename U>
	uint64_t BindPropertySignal(std::string property_name, std::function<void(U&&)> callback)
	{
		auto itr = m_propertyTable.find(property_name);
		if (itr != m_propertyTable.end())
		{
			return itr->second->Bind<U>(callback);
		}
		else
		{
			LOG_ERROR("property is not found: [%s]", property_name.c_str());
			_ASSERT(false);
		}
		return 0;
	}

	template<typename U>
	void UnBindPropertySignal(std::string property_name, uint64_t slot)
	{
		auto itr = m_propertyTable.find(property_name);
		if (itr != m_propertyTable.end())
		{
			return itr->second->UnBind<U>(slot);
		}
		else
		{
			LOG_ERROR("property is not found: [%s]", property_name.c_str());
			_ASSERT(false);
		}
	}
};