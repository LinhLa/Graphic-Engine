#include "stdafx.h"
#include <string>
#include <LightingProperty.h>

std::string parse_name(GLint index, std::string property_name)
{
	property_name.insert(property_name.find_first_of('.'), "[" + std::to_string(index) + "]");
	return property_name;
}

template<class T>
void AddProperty(MaterialPtr pMaterial, std::string name, IPropertyPtr property)
{
	auto source = std::dynamic_pointer_cast<Property<float>>(property);
	if (pMaterial->IsPropertyExist(name))
	{
		pMaterial->SetPropertyValue(name, source->GetValue());
	}
	else
	{
		auto target = Property<float>::create(name, property->GetValue<T>(), property->GetType());
		target->setUpper(source->getLower());
		target->setUpper(source->getUpper());
		pMaterial->AddProperty(name, target);
	}
}

PointLightProperty::PointLightProperty(PropertyTable* pPropertyTable)
{
	if (!pPropertyTable)
	{
		_ASSERT(false);
	}
	m_pPropertyTable = pPropertyTable;
}

PointLightProperty::~PointLightProperty() {}

void PointLightProperty::init()
{
	m_pPropertyTable->AddProperty(LIGHT_TYPE, Property<glm::i32>::create(LIGHT_TYPE, POINT_LIGHT, GL_INT));
}

void  PointLightProperty::SetDiffuseColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(POINT_LIGHT_DIFFUSE, std::move(color));
}

glm::vec4  PointLightProperty::GetDiffuseColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(POINT_LIGHT_DIFFUSE);
}


void  PointLightProperty::SetAmbientColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(POINT_LIGHT_AMBIENT, std::move(color));
}

glm::vec4  PointLightProperty::GetAmbientColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(POINT_LIGHT_AMBIENT);
}


void  PointLightProperty::SetSpecularColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(POINT_LIGHT_SPECULAR, std::move(color));
}

glm::vec4 PointLightProperty::GetSpecularColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(POINT_LIGHT_SPECULAR);
}

glm::vec3 PointLightProperty::GetPosition() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(POINT_LIGHT_POSITION);
}


void PointLightProperty::SetProperty(MaterialPtr pMaterial, GLint count)
{
	if (!pMaterial)
	{
		return;
	}

	auto list = {
		POINT_LIGHT_COLOR,
		POINT_LIGHT_AMBIENT,
		POINT_LIGHT_DIFFUSE,
		POINT_LIGHT_SPECULAR,
		POINT_LIGHT_POSITION,
		POINT_LIGHT_CONSTANT,
		POINT_LIGHT_LINEAR,
		POINT_LIGHT_QUADRATIC
	};

	for (auto& light_property : list)
	{
		if (m_pPropertyTable->IsPropertyExist(light_property))
		{
			auto name = parse_name(count, light_property);
			if (pMaterial->IsPropertyExist(name))
			{
				auto property = m_pPropertyTable->GetProperty(light_property);
				property->SetValue(pMaterial->GetProperty(name));
			}
			else
			{
				pMaterial->AddProperty(name, m_pPropertyTable->GetProperty(light_property)->clone());
			}
		}
	}
}

SpotLightProperty::SpotLightProperty(PropertyTable* pPropertyTable)
{
	if (!pPropertyTable)
	{
		_ASSERT(false);
	}
	m_pPropertyTable = pPropertyTable;
}

SpotLightProperty::~SpotLightProperty() {}

void SpotLightProperty::init()
{
	m_pPropertyTable->AddProperty(LIGHT_TYPE, Property<glm::i32>::create(LIGHT_TYPE, SPOT_LIGHT, GL_INT));
}

void  SpotLightProperty::SetDiffuseColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(SPOT_LIGHT_DIFFUSE, std::move(color));
}

glm::vec4  SpotLightProperty::GetDiffuseColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(SPOT_LIGHT_DIFFUSE);
}


void  SpotLightProperty::SetAmbientColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(SPOT_LIGHT_AMBIENT, std::move(color));
}

glm::vec4  SpotLightProperty::GetAmbientColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(SPOT_LIGHT_AMBIENT);
}


void  SpotLightProperty::SetSpecularColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(SPOT_LIGHT_SPECULAR, std::move(color));
}

glm::vec4 SpotLightProperty::GetSpecularColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(SPOT_LIGHT_SPECULAR);
}

glm::vec3 SpotLightProperty::GetPosition() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(SPOT_LIGHT_POSITION);
}

glm::vec3 SpotLightProperty::GetDirection() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(SPOT_LIGHT_DIRECTION);
}

void SpotLightProperty::SetProperty(MaterialPtr pMaterial, GLint count)
{
	if (!pMaterial)
	{
		return;
	}

	auto list = {
		SPOT_LIGHT_AMBIENT,
		SPOT_LIGHT_DIFFUSE,
		SPOT_LIGHT_SPECULAR,
		SPOT_LIGHT_POSITION,
		SPOT_LIGHT_CUTOFF,
		SPOT_LIGHT_INNER_CUTOFF,
		SPOT_LIGHT_OUTER_CUTOFF,
		SPOT_LIGHT_DIRECTION
	};

	for (auto& light_property : list)
	{
		if (m_pPropertyTable->IsPropertyExist(light_property))
		{
			auto name = parse_name(count, light_property);
			if (pMaterial->IsPropertyExist(name))
			{
				auto property = m_pPropertyTable->GetProperty(light_property);
				property->SetValue(pMaterial->GetProperty(name));
			}
			else
			{
				pMaterial->AddProperty(name, m_pPropertyTable->GetProperty(light_property)->clone());
			}
		}
	}
}


DirectionalLightProperty::DirectionalLightProperty(PropertyTable* pPropertyTable)
{
	if (!pPropertyTable)
	{
		_ASSERT(false);
	}
	m_pPropertyTable = pPropertyTable;
}

DirectionalLightProperty::~DirectionalLightProperty() {}

void DirectionalLightProperty::init()
{
	m_pPropertyTable->AddProperty(LIGHT_TYPE, Property<glm::i32>::create(LIGHT_TYPE, DIRECTIONAL_LIGHT, GL_INT));
}

void  DirectionalLightProperty::SetDiffuseColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(DIRECTIONAL_LIGHT_DIFFUSE, std::move(color));
}

glm::vec4  DirectionalLightProperty::GetDiffuseColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(DIRECTIONAL_LIGHT_DIFFUSE);
}


void  DirectionalLightProperty::SetAmbientColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(DIRECTIONAL_LIGHT_AMBIENT, std::move(color));
}

glm::vec4  DirectionalLightProperty::GetAmbientColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(DIRECTIONAL_LIGHT_AMBIENT);
}


void  DirectionalLightProperty::SetSpecularColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(DIRECTIONAL_LIGHT_SPECULAR, std::move(color));
}

glm::vec4 DirectionalLightProperty::GetSpecularColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(DIRECTIONAL_LIGHT_SPECULAR);
}

glm::vec3 DirectionalLightProperty::GetDirection() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(DIRECTIONAL_LIGHT_DIRECTION);
}

void DirectionalLightProperty::SetProperty(MaterialPtr pMaterial, GLint count)
{
	if (!pMaterial)
	{
		return;
	}

	auto list = {
		DIRECTIONAL_LIGHT_COLOR,
		DIRECTIONAL_LIGHT_AMBIENT,
		DIRECTIONAL_LIGHT_DIFFUSE,
		DIRECTIONAL_LIGHT_SPECULAR,
		DIRECTIONAL_LIGHT_DIRECTION
	};

	for (auto& light_property : list)
	{
		if (m_pPropertyTable->IsPropertyExist(light_property))
		{
			auto name = parse_name(count, light_property);
			if (pMaterial->IsPropertyExist(name))
			{
				auto property = m_pPropertyTable->GetProperty(light_property);
				property->SetValue(pMaterial->GetProperty(name));
			}
			else
			{
				pMaterial->AddProperty(name, m_pPropertyTable->GetProperty(light_property)->clone());
			}
		}
	}
}
