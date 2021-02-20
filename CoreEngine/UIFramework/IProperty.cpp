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

	if (IsPropertyExist(property_name))
	{
		LOG_DEBUG("property[%s] is exist", property_name.c_str());
	}

	m_propertyTable[property_name] = pProperty;
	bResult = true;

	return bResult;
}

void PropertyTable::RemoveProperty(std::string property_name)
{
	m_propertyTable.erase(property_name);
}