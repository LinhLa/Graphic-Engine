#include "stdafx.h"
#include <string>
#include <GLProperty.h>
#include "PropertyDefine.h"
#include "UIHelper.h"
#include "Configuration.h"


GLProperty::GLProperty(PropertyTable* pPropertyTable)
{
	if (!pPropertyTable)
	{
		_ASSERT(false);
	}
	m_pPropertyTable = pPropertyTable;
}

GLProperty::~GLProperty() {}

void GLProperty::init()
{
	m_pPropertyTable->AddProperty(BLEND_INDENSITY, Property<float>::create(BLEND_INDENSITY, 1.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(VIEW_PORT, Property<glm::i32vec4>::create(VIEW_PORT, glm::i32vec4(0,0,Configuration::GetInstance()->width,Configuration::GetInstance()->height),GL_INT_VEC4));
}

float GLProperty::GetBlendIndensity() const
{
	return m_pPropertyTable->GetPropertyValue<float>(BLEND_INDENSITY);
}

void GLProperty::SetBlendIndensity(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(BLEND_INDENSITY, std::move(value));
}

glm::i32vec4 GLProperty::GetViewPort() const
{
	return m_pPropertyTable->GetPropertyValue<glm::i32vec4>(VIEW_PORT);
}

void GLProperty::SetViewPort(glm::i32vec4 value)
{
	m_pPropertyTable->SetPropertyValue<glm::i32vec4>(VIEW_PORT, std::move(value));
}