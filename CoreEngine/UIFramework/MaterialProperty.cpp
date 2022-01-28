#include "stdafx.h"
#include <string>
#include <MaterialProperty.h>
#include <Material.h>
#include <Library.h>
#include "UIHelper.h"

MaterialProperty::MaterialProperty(PropertyTable* pPropertyTable)
{
	if (!pPropertyTable)
	{
		_ASSERT(false);
	}
	m_pPropertyTable = pPropertyTable;
}

MaterialProperty::~MaterialProperty() {}

void MaterialProperty::init()
{
	m_pPropertyTable->AddProperty(MATERIAL_BLENDING_MODE, Property<glm::i32>::create(MATERIAL_BLENDING_MODE, 0, GL_INT));

	auto textureDiffuseID = Library::GetInstance()->get<GLTexture>(TEXTURE_DIFFUSE_DEFAULT)->getID();
	auto textureSpecularID = Library::GetInstance()->get<GLTexture>(TEXTURE_SPECULAR_DEFAULT)->getID();
	auto texturenormalID = Library::GetInstance()->get<GLTexture>(TEXTURE_NORMAL_DEFAULT)->getID();
	m_pPropertyTable->AddProperty(MATERIAL_DIFFUSE_MAP, Property<glm::u32>::create(MATERIAL_DIFFUSE_MAP, std::move(textureDiffuseID), GL_SAMPLER_2D));
	m_pPropertyTable->AddProperty(MATERIAL_SPECULAR_MAP, Property<glm::u32>::create(MATERIAL_SPECULAR_MAP, std::move(textureSpecularID), GL_SAMPLER_2D));
	m_pPropertyTable->AddProperty(MATERIAL_DISPLACEMENT_MAP, Property<glm::u32>::create(MATERIAL_DISPLACEMENT_MAP, std::move(texturenormalID), GL_SAMPLER_2D));
	m_pPropertyTable->AddProperty(MATERIAL_NORMAL_MAP, Property<glm::u32>::create(MATERIAL_NORMAL_MAP, std::move(texturenormalID), GL_SAMPLER_2D));
	m_pPropertyTable->AddProperty(MATERIAL_ROUGHNESS_MAP, Property<glm::u32>::create(MATERIAL_ROUGHNESS_MAP, std::move(texturenormalID), GL_SAMPLER_2D));

	m_pPropertyTable->AddProperty(MATERIAL_DIFFUSE, Property<glm::vec4>::create(MATERIAL_DIFFUSE, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), GL_FLOAT_VEC4));
	m_pPropertyTable->AddProperty(MATERIAL_SPECULAR, Property<glm::vec4>::create(MATERIAL_SPECULAR, glm::vec4(1.0f), GL_FLOAT_VEC4));
	m_pPropertyTable->AddProperty(MATERIAL_AMBIENT, Property<glm::vec4>::create(MATERIAL_AMBIENT, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), GL_FLOAT_VEC4));
	m_pPropertyTable->AddProperty(MATERIAL_SHININESS, Property<float>::create(MATERIAL_SHININESS, 128.0f, GL_FLOAT));
}

void  MaterialProperty::SetDiffuseColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(MATERIAL_DIFFUSE, std::move(color));
}

glm::vec4  MaterialProperty::GetDiffuseColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(MATERIAL_DIFFUSE);
}


void  MaterialProperty::SetAmbientColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(MATERIAL_AMBIENT, std::move(color));
}

glm::vec4  MaterialProperty::GetAmbientColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(MATERIAL_AMBIENT);
}


void  MaterialProperty::SetSpecularColor(glm::vec4 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec4>(MATERIAL_SPECULAR, std::move(color));
}

glm::vec4 MaterialProperty::GetSpecularColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(MATERIAL_SPECULAR);
}


void  MaterialProperty::SetDiffuseMap(glm::u32 texture)
{
	m_pPropertyTable->SetPropertyValue<glm::u32>(MATERIAL_DIFFUSE_MAP, std::move(texture));
}

glm::u32  MaterialProperty::GetDiffuseMap() const
{
	return m_pPropertyTable->GetPropertyValue<glm::u32>(MATERIAL_DIFFUSE_MAP);
}


void  MaterialProperty::SetSpecularMap(glm::u32 texture)
{
	m_pPropertyTable->SetPropertyValue<glm::u32>(MATERIAL_SPECULAR_MAP, std::move(texture));
}

glm::u32  MaterialProperty::GetSpecularMap() const
{
	return m_pPropertyTable->GetPropertyValue<glm::u32>(MATERIAL_SPECULAR_MAP);
}

void  MaterialProperty::SetShininess(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(MATERIAL_SHININESS, std::move(value));
}

float  MaterialProperty::GetShininess() const
{
	return m_pPropertyTable->GetPropertyValue<float>(MATERIAL_SHININESS);
}
