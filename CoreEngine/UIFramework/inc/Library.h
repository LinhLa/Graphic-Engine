#pragma once
#include <SDL.h>
#include <unordered_map>
#include "creator.h"
#include <SingletonHolder.h>

class IDataContainer
{
public:
	IDataContainer(){}
	virtual ~IDataContainer(){}
	virtual const std::type_info& getTypeInfo() const = 0;
};

typedef std::shared_ptr<IDataContainer> DataContainerPtr;

template<class T>
class DataConcrete final: public IDataContainer, public creator<DataConcrete<T>>
{
private:
	std::unordered_map<std::string, std::shared_ptr<T>> m_dataTable;
protected:
	DataConcrete(){}
public:
	friend class creator<DataConcrete<T>>;
	virtual ~DataConcrete(){}

	const std::type_info& getTypeInfo() const override
	{
		return typeid(T);
	}

	void add(const std::string& name, std::shared_ptr<T> p)
	{
		if (p)
		{
			m_dataTable.insert({ name, p });
		}
	}

	std::shared_ptr<T> get(const std::string& name)
	{
		auto itr_result = m_dataTable.find(name);
		if (itr_result != m_dataTable.end())
		{
			return itr_result->second;
		}
		else
		{
			LOG_DEBUG("%s not found [%s]", typeid(T).name(), name.c_str());
		}
		return nullptr;
	}
};

class Library final: public SingletonHolder<Library>
{
private:
	std::list<DataContainerPtr> m_listDataContainer;
protected:
	Library();
	~Library();
public:

	template<class T>
	void add(const std::string& name, std::shared_ptr<T> p)
	{
		DataContainerPtr pContainer = nullptr;
		for(auto &pInstance: m_listDataContainer)
		{
			if (nullptr != std::dynamic_pointer_cast<DataConcrete<T>>(pInstance))
			{
				pContainer = pInstance;
				break;
			}
		}

		if (!pContainer)
		{
			pContainer = DataConcrete<T>::create();
			m_listDataContainer.push_back(pContainer);
			
		}
		std::dynamic_pointer_cast<DataConcrete<T>>(pContainer)->add(name, p);
	}

	template<class T>
	std::shared_ptr<T> get(const std::string& name)
	{
		DataContainerPtr pContainer = nullptr;
		for(auto &pInstance: m_listDataContainer)
		{
			if (nullptr != std::dynamic_pointer_cast<DataConcrete<T>>(pInstance))
			{
				pContainer = pInstance;
				break;
			}
		}
		
		if (!pContainer)
		{
			pContainer = DataConcrete<T>::create();
			m_listDataContainer.push_back(pContainer);
			
		}
		return std::dynamic_pointer_cast<DataConcrete<T>>(pContainer)->get(name);
	}
	friend class SingletonHolder<Library>;
};