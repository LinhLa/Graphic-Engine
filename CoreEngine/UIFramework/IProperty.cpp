#include "stdafx.h"
#include "IProperty.h"

const std::map<std::string, uint8_t> property_type_map =
{
	{"STRING", STRING},
	{"INT", INT},
	{"BOOL", BOOL},
	{"FLOAT", FLOAT},
	{"VEC2", VEC2},
	{"VEC3", VEC3},
	{"VEC4", VEC4},
	{"ARRAY_INT", ARRAY_INT },
};
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