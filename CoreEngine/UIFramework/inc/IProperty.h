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
#ifdef OPENGL_RENDERING
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#endif

#include <glm/glm.hpp>

class ImGuiShader;
class AnimationProperty;

/**
 * @brief      forward declairation
 */
template<class T> class Property;

class IProperty;
typedef std::shared_ptr<IProperty> IPropertyPtr;

/**
 * @brief      This class describes a property interface.
 */
class IProperty : public std::enable_shared_from_this<IProperty>
{
public:
	IProperty() {}
	virtual ~IProperty() {}
	virtual std::string getPropertyName() const = 0;
	virtual const std::type_info& getTypeInfo() const = 0;
	virtual int32_t GetType() const = 0;
	virtual IPropertyPtr clone() = 0;
	virtual void SetValue(IPropertyPtr pProperty) = 0;
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
		return dynamic_cast<Property<U> const*>(this);
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

/**
 * @brief      This class describes a property.
 *
 * @tparam     T     { description }
 */
template<class T>
class Property final : public IProperty, public creator<Property<T>>
{
private:
	std::string 	m_property_name;
	T 				m_value;
	T				m_upper;
	T				m_lower;
	uint32_t 		m_type = 0;
public:
	std::shared_ptr<Signal<T>> 	m_pSignalValueChange;
protected:
	friend class Property<T>;
	Property(std::string name, T&& value = T{}, uint32_t uType = GL_INVALID_ENUM) :m_property_name(name), m_value(value), m_type(uType)
	{
		m_pSignalValueChange = Signal<T>::create();
	}
public:
	virtual ~Property() {}

	friend class creator<Property<T>>;

	std::string getPropertyName() const override
	{
		return m_property_name;
	}

	const std::type_info& getTypeInfo() const override
	{
		return typeid(T);
	}

	IPropertyPtr clone() override
	{
		auto p = Property<T>::create(m_property_name);
		p->m_value = m_value;
		p->m_upper = m_upper;
		p->m_lower = m_lower;
		p->m_type = m_type;
		return p;
	}

	void SetValue(IPropertyPtr pProperty) override
	{
		auto p = std::dynamic_pointer_cast<Property<T>>(pProperty);
		if (nullptr != p)
		{
			p->m_value = m_value;
		}
		else
		{
			LOG_DEBUG("type mismatch Source[%s] Target[%s]", m_property_name.c_str(), p->m_property_name.c_str());
		}
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

	int32_t GetType() const
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

	T getUpper() const
	{
		return m_upper;
	}

	T getLower() const
	{
		return m_lower;
	}

	T* getPointer()
	{
		return &m_value;
	}

	T& getRef()
	{
		return m_value;
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

	friend class ImGuiShader;
	friend class AnimationProperty;
public:
	virtual ~PropertyTable();

	bool IsPropertyExist(std::string);

	bool AddProperty(std::string, IPropertyPtr);
	IPropertyPtr GetProperty(std::string);
	void RemoveProperty(std::string);

	void importProperty(std::shared_ptr<PropertyTable> pTable);
	void shareProperty(std::shared_ptr<PropertyTable> pTable);
	void setProperty(std::shared_ptr<PropertyTable> pTable);

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
			itr->second->Visitor<U, V>(std::forward<V>(visitor));
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