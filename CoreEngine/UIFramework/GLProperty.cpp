#include "stdafx.h"
#include <string>
#include <GLProperty.h>
#include "PropertyDefine.h"
#include "UIHelper.h"
GLProperty::GLProperty(PropertyTable* propertyTable):m_PropertyTable(propertyTable)
{
	if (!m_PropertyTable)
	{
		throw std::logic_error("invalid_argument");
	}
}

GLProperty::~GLProperty() {}

GLProperty& GLProperty::operator=(const GLProperty& rhs)
{
	SetBlendIndensity(rhs.GetBlendIndensity());
	return *this;
}

void GLProperty::init()
{
	m_PropertyTable->AddProperty(BLEND_INDENSITY, Property<float>::create(BLEND_INDENSITY));
	//<Set default value
	m_PropertyTable->SetPropertyValue<float>(BLEND_INDENSITY, std::move(1.0F));
}

void GLProperty::AddTexture(GLTexturePtr pTexture)
{
	if (pTexture)
	{
		m_TextureList.push_back(pTexture);
	}
}

std::vector<GLTexturePtr> GLProperty::GetTextureList()
{
	return m_TextureList;
}

float GLProperty::GetBlendIndensity() const
{
	return m_PropertyTable->GetPropertyValue<float>(BLEND_INDENSITY);
}

void GLProperty::SetBlendIndensity(float value)
{
	m_PropertyTable->SetPropertyValue<float>(BLEND_INDENSITY, std::move(value));
}
