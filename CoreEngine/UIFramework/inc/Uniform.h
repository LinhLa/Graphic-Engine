#pragma once
#include <type_traits>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <string>
#include "creator.h"
#include "log.h"
#include "GLTypeDictionary.h"
#include "UIObject.h"
#include "Library.h"
#include "Material.h"
class IUniform;
template<typename T> class Uniform;
std::shared_ptr<IUniform> create(std::string name, GLuint index, GLenum type, GLint size);

class IUniform:public std::enable_shared_from_this<IUniform>
{
public:
	IUniform(){}
	virtual ~IUniform(){}
	virtual std::string getName() const = 0;
	virtual GLenum getType() const = 0;
	virtual const std::type_info& getTypeInfo() const = 0;
	virtual void debug() = 0;

	virtual void set(IPropertyPtr pProperty) = 0;
	virtual void sync(std::shared_ptr<PropertyTable> pObject) = 0;
	void glUniform(GLint location, void* param = nullptr);

	template<class U>
	U get() const
	{
		return DynamicTypeCheck<U>()->get();
	}
private:
	template<typename U>
	Uniform<U>* DynamicTypeCheck()
	{
		if (typeid(U).hash_code() != getTypeInfo().hash_code())
		{
			throw std::logic_error("invalid_argument");
		}
		return dynamic_cast<Uniform<U>*>(this);
	}

	template<typename U>
	Uniform<U> const* DynamicTypeCheck() const
	{
		if (typeid(U).hash_code() != getTypeInfo().hash_code())
		{
			throw std::logic_error("invalid_argument");
		}
		return dynamic_cast<Uniform<U> const *>(this);
	}
};

template<class T>
class Uniform final: public creator<Uniform<T>>, public IUniform
{
private:
	std::string		m_name;
	GLuint 			m_index = 0;
	GLenum 			m_type = 0;
	GLint 			m_size = 0;
	T 				m_data;
protected:
	Uniform(std::string name, GLuint index, GLenum type, GLint size):m_name(name), m_index(index), m_type(type), m_size(size)
	{}
public:
	virtual ~Uniform(){}
	friend class creator<Uniform>;

	std::string getName() const
	{
		return m_name;
	}
	const std::type_info& getTypeInfo() const override
	{
		return typeid(T);
	}

	GLenum getType() const
	{
		return m_type;
	}

	void set(T&& value)
	{
		m_data = value;
	}

	T get() const
	{
		return m_data;
	}

	void set(IPropertyPtr pProperty)
	{
		if (nullptr != std::dynamic_pointer_cast<Property<T>>(pProperty))
		{
			set(std::dynamic_pointer_cast<Property<T>>(pProperty)->GetValue());
		}
		else
		{
			LOG_DEBUG("type mismatch Uniform[%s] Property[%s]", m_name.c_str(), pProperty->getPropertyName().c_str());
		}
	}

	void sync(std::shared_ptr<PropertyTable> pObject)
	{
		if (GL_SAMPLER_2D == m_type)
		{
			auto pTexture = Library::GetInstance()->get<GLTexture>(TEXTURE_DIFFUSE_DEFAULT);
			pObject->AddProperty(this->getName(), Property<glm::u32>::create(this->getName(), pTexture->getID(), m_type));
		}
		else
		{
			pObject->AddProperty(this->getName(), Property<T>::create(this->getName(), T(), m_type));
		}
	}

	void debug()
	{
		LOG_DEBUG("[%s]: index[%d] type[%s] size[%d]" , m_name.c_str(), m_index, GLSL_TYPE_DICTIONARY.at(m_type).c_str(), m_size);
	}
};

typedef std::shared_ptr<IUniform> IUniformPtr;