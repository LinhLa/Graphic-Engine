#include "stdafx.h"
#include <TextProperty.h>
#include "PropertyDefine.h"

TextProperty::TextProperty(PropertyTable* pPropertyTable)
{
	if (!pPropertyTable)
	{
		_ASSERT(false);
	}
	m_pPropertyTable = pPropertyTable;
}

TextProperty::~TextProperty()
{}

void TextProperty::init()
{
	m_pPropertyTable->AddProperty(TEXT, Property<std::string>::create(TEXT, std::string(), GL_ARRAY_BUFFER));
	m_pPropertyTable->AddProperty(FONT_NAME, Property<std::string>::create(FONT_NAME, std::string(), GL_ARRAY_BUFFER));
	m_pPropertyTable->AddProperty(FONT_SIZE, Property<int32_t>::create(FONT_SIZE, 12, GL_INT));
#ifdef OPENGL_RENDERING
	m_pPropertyTable->AddProperty(FONT_COLOR, Property<glm::vec3>::create(FONT_COLOR, glm::vec3(1.0f), GL_FLOAT_VEC3));
#else
	m_pPropertyTable->AddProperty(FONT_COLOR, Property<SDL_Color>::create(FONT_COLOR));
#endif
}

void  TextProperty::SetText(const char* text)
{
	m_pPropertyTable->SetPropertyValue<std::string>(TEXT, std::move(std::string(text)));
}

std::string  TextProperty::GetText() const
{
	return m_pPropertyTable->GetPropertyValue<std::string>(TEXT);
}

void  TextProperty::SetFontName(const char* font_name)
{
	m_pPropertyTable->SetPropertyValue<std::string>(FONT_NAME, std::move(std::string(font_name)));
}

std::string  TextProperty::GetFontName() const
{
	return m_pPropertyTable->GetPropertyValue<std::string>(FONT_NAME);
}

void  TextProperty::SetFontSize(int32_t size)
{
	m_pPropertyTable->SetPropertyValue<int32_t>(FONT_SIZE, std::move(size));
}

int32_t  TextProperty::GetFontSize() const
{
	return m_pPropertyTable->GetPropertyValue<int32_t>(FONT_SIZE);
}
#ifndef OPENGL_RENDERING
void  TextProperty::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
{
	m_pPropertyTable->SetPropertyValue<SDL_Color>(FONT_COLOR, std::move(SDL_Color{ r,g,b,alpha }));
}

void  TextProperty::SetColor(SDL_Color color)
{
	m_pPropertyTable->SetPropertyValue<SDL_Color>(FONT_COLOR, std::move(color));
}

SDL_Color TextProperty::GetColor() const
{
	return m_pPropertyTable->GetPropertyValue<SDL_Color>(FONT_COLOR);
}
#else
void TextProperty::SetColor(glm::vec3 color)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(FONT_COLOR, std::move(color));
}

glm::vec3 TextProperty::GetColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(FONT_COLOR);
}
#endif