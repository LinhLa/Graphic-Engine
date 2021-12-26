#include "stdafx.h"
#include <string>
#include <MaterialProperty.h>
#include "UIHelper.h"

MaterialProperty::MaterialProperty(PropertyTable* propertyTable):m_PropertyTable(propertyTable)
{
	if (!m_PropertyTable)
	{
		throw std::logic_error("invalid_argument");
	}
}

MaterialProperty::~MaterialProperty() {}

MaterialProperty& MaterialProperty::operator=(const MaterialProperty& rhs)
{
	return *this;
}

void MaterialProperty::init()
{
}