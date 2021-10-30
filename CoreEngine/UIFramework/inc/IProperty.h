#pragma once
#include <exception>
#include <unordered_map>
#include <string.h>
#include <typeinfo>
#include <memory>
#include <sstream>
#include <map>
#include "creator.h"
#include "Signal.h"
#include "log.h"
#include "utils.h"
#include <SDL.h>
#include <glm/glm.hpp>

enum PROPERTY_TYPE: int32_t
{
	UNDEFINE = 0U,
	INT,
	FLOAT,
	BOOL,
	STRING,
	GLM_VEC4,
	GLM_VEC3,
	VEC4,
	VEC3,
	VEC2,
	URL_TEXTURE,
	URL_SHADER,
	URL_PROGRAM,
	URL_MESH,
	URL_MODEL,
	ARRAY_INT,
};


class PropertyType final: public creator<PropertyType>
{
public:
	friend class creator<PropertyType>;
	uint8_t m_type;
	PropertyType(uint8_t type) :m_type(type) {}
	~PropertyType() {}
};

typedef std::shared_ptr<PropertyType> PropertyTypePtr;
extern const std::map<std::string, uint8_t> property_type_map;
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
	T				m_upper;
	T				m_lower;
	int32_t 		m_type = 0;
	bool 			m_isAnimate = false;
public:
	std::shared_ptr<Signal<T>> 	m_pSignalValueChange;
protected:
	Property(std::string name, T&& value = T{}, int32_t uType = UNDEFINE) :m_property_name(name), m_value(value), m_type(uType)
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

	uint8_t GetType() const
	{
		return m_type;
	}

	void setUpper(T upper)
	{
		m_upper = upper;
	}

	void setLower(T lower)
	{
		m_lower = lower;
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
	IPropertyPtr GetProperty(std::string);
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