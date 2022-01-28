#pragma once
#include <unordered_map>
#include <vector>
#include "creator.h"
#include <SingletonHolder.h>
#include <ISignalMethod.h>
#include "LibrarySignalMethod.h"
#include "SignalDefine.h"

class IDataContainer
{
public:
	IDataContainer(){}
	virtual ~IDataContainer(){}
	virtual const std::type_info& getTypeInfo() const = 0;
};

typedef std::shared_ptr<IDataContainer> DataContainerPtr;

template<class T>
class DataConcrete final: public IDataContainer, public SignalTable, public SignalMethodTable, public creator<DataConcrete<T>>, public std::enable_shared_from_this<DataConcrete<T>>
{
private:
	std::unordered_map<std::string, std::shared_ptr<T>> m_dataTable;
protected:
	DataConcrete()
	{
		AddSignalMethodObj(LibrarySignalMethod::create(dynamic_cast<SignalTable*>(this)));
	}
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

			//<broadcast signal on update
			OnSignal(ON_UPDATE_SIGNAL, VoidType{});
		}
	}

	void remove(const std::string& name)
	{
		auto itr = m_dataTable.find(name);
		if (itr != m_dataTable.end())
		{
			m_dataTable.erase(itr);
			//<broadcast signal on update
			OnSignal(ON_UPDATE_SIGNAL, VoidType{});
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

	std::vector<std::string> get()
	{
		std::vector<std::string> list;
		for (auto& item : m_dataTable)
		{
			list.push_back(item.first);
		}
		return list;
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

	template<class T>
	void remove(const std::string& name)
	{
		DataContainerPtr pContainer = nullptr;
		for (auto& pInstance : m_listDataContainer)
		{
			if (nullptr != std::dynamic_pointer_cast<DataConcrete<T>>(pInstance))
			{
				pContainer = pInstance;
				break;
			}
		}

		if (pContainer)
		{
			std::dynamic_pointer_cast<DataConcrete<T>>(pContainer)->remove(name);

		}
	}

	template<class T>
	std::vector<std::string> get()
	{
		std::vector<std::string> list;
		for(auto &pInstance: m_listDataContainer)
		{
			if (nullptr != std::dynamic_pointer_cast<DataConcrete<T>>(pInstance))
			{
				list = std::dynamic_pointer_cast<DataConcrete<T>>(pInstance)->get();
				break;
			}
		}
		return list;
	}

	template<class T>
	std::shared_ptr<SignalTable> getSignalTable()
	{
		std::shared_ptr<SignalTable> pSignalTable = nullptr;
		for (auto& pInstance : m_listDataContainer)
		{
			if (nullptr != std::dynamic_pointer_cast<DataConcrete<T>>(pInstance))
			{
				pSignalTable = std::dynamic_pointer_cast<SignalTable>(pInstance);
				break;
			}
		}
		return pSignalTable;
	}

	friend class SingletonHolder<Library>;
};