#include "stdafx.h"
#include "IProperty.h"

const std::map<std::string, uint8_t> property_type_map =
{
	{"STRING", STRING},
	{"INT", INT},
	{"INT", INT},
	{"STRING", STRING},
	{"BOOL", BOOL},
	{"INT", INT},
	{"INT", INT},
	{"ARRAY_INT", ARRAY_INT},
	{"STRING", STRING},
	{"VEC3", VEC3},
	{"VEC3", VEC3},
	{"VEC3", VEC3},
	{"FLOAT", FLOAT},
	{"FLOAT", FLOAT},
	{"BOOL", BOOL},
	{"FLOAT", FLOAT},
	{"VEC2", VEC2},
	{"VEC3", VEC3},
	{"FLOAT", FLOAT},
	{"INT", INT},
	{"BOOL", BOOL},
	{"STRING", STRING},
	{"STRING", STRING},
	{"INT", INT},
	{"VEC3", VEC3},
	{"URL_PROGRAM", URL_PROGRAM},
	{"URL_MESH", URL_MESH},
	{"FLOAT", FLOAT},
	{"URL_TEXTURE", URL_TEXTURE},
	{"FLOAT", FLOAT},
	{"VEC4", VEC4},
	{"URL_TEXTURE", URL_TEXTURE},
	{"URL_TEXTURE", URL_TEXTURE}
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