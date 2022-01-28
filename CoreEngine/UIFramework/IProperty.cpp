#include "stdafx.h"
#include "IProperty.h"

PropertyTable::PropertyTable()
{}

PropertyTable::~PropertyTable()
{}

bool PropertyTable::IsPropertyExist(std::string property_name)
{
	return (m_propertyTable.find(property_name) != m_propertyTable.end());
}

bool PropertyTable::AddProperty(std::string property_name, IPropertyPtr pProperty)
{
	bool bResult = false;
	if (!pProperty)
	{
		LOG_ERROR("null pointer");
		return bResult;
	}

	m_propertyTable[property_name] = pProperty;
	bResult = true;

	return bResult;
}

IPropertyPtr PropertyTable::GetProperty(std::string property_name)
{
	IPropertyPtr pReturn = nullptr;
	auto itr = m_propertyTable.find(property_name);
	if (itr != m_propertyTable.end())
	{
		pReturn = itr->second;
	}
	return pReturn;
}

void PropertyTable::RemoveProperty(std::string property_name)
{
	m_propertyTable.erase(property_name);
}


/**
 * @brief      { function_description }
 * Only import property which doen't exist
 * @param[in]  pTable  The table
 */
void PropertyTable::importProperty(std::shared_ptr<PropertyTable> pTable)
{
	if (!pTable)
	{
		LOG_ERROR("null pointer");
		return;
	}

	for (auto& item : pTable->m_propertyTable)
	{
		auto item_iter = m_propertyTable.find(item.first);
		if (item_iter == m_propertyTable.end())
		{
			m_propertyTable.insert(std::make_pair(item.first, item.second->clone()));
		}
		else{}
	}
}

/**
 * @brief      : target object adopt property from source object
 * caution     : target property which exist will replaced and may crash if it conten any signal
 * @param[in]  pTable  The table
 */
void PropertyTable::shareProperty(std::shared_ptr<PropertyTable> pTable)
{
	if (!pTable)
	{
		LOG_ERROR("null pointer");
		return;
	}

	for (auto& item : pTable->m_propertyTable)
	{
		m_propertyTable[item.first] = item.second;
	}
}

/*
* Usaged: Set property to pTable
*/
void PropertyTable::setProperty(std::shared_ptr<PropertyTable> pTable)
{
	if (!pTable)
	{
		LOG_ERROR("null pointer");
		return;
	}

	for (auto& item : m_propertyTable)
	{
		auto pProperty = pTable->GetProperty(item.first);
		if (pProperty)
		{
			item.second->SetValue(pProperty);
		}
	}
}