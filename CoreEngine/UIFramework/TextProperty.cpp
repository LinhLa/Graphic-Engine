#include "stdafx.h"
#include <TextProperty.h>
#include "PropertyDefine.h"

TextProperty::TextProperty(PropertyTable* propertyTable):m_PropertyTable(propertyTable)
{
	if (!m_PropertyTable)
	{
		throw std::logic_error("invalid_argument");
	}
}

TextProperty::~TextProperty() {}

TextProperty& TextProperty::operator=(const TextProperty& rhs)
{
	SetText(rhs.GetText().c_str());
	SetFontName(rhs.GetFontName().c_str());
	SetFontSize(rhs.GetFontSize());
	SetColor(rhs.GetColor());
	return *this;
}

void TextProperty::init()
{
	m_PropertyTable->AddProperty(TEXT, Property<std::string>::create(TEXT));
	m_PropertyTable->AddProperty(FONT_NAME, Property<std::string>::create(FONT_NAME));
	m_PropertyTable->AddProperty(FONT_SIZE, Property<int32_t>::create(FONT_SIZE));
	m_PropertyTable->AddProperty(FONT_COLOR, Property<SDL_Color>::create(FONT_COLOR));
}

void  TextProperty::SetText(const char* text)
{
	m_PropertyTable->SetPropertyValue<std::string>(TEXT, std::move(std::string(text)));
}

std::string  TextProperty::GetText() const
{
	return m_PropertyTable->GetPropertyValue<std::string>(TEXT);
}

void  TextProperty::SetFontName(const char* font_name)
{
	m_PropertyTable->SetPropertyValue<std::string>(FONT_NAME, std::move(std::string(font_name)));
}

std::string  TextProperty::GetFontName() const
{
	return m_PropertyTable->GetPropertyValue<std::string>(FONT_NAME);
}

void  TextProperty::SetFontSize(int32_t size)
{
	m_PropertyTable->SetPropertyValue<int32_t>(FONT_SIZE, std::move(size));
}

int32_t  TextProperty::GetFontSize() const
{
	return m_PropertyTable->GetPropertyValue<int32_t>(FONT_SIZE);
}

void  TextProperty::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
{
	m_PropertyTable->SetPropertyValue<SDL_Color>(FONT_COLOR, std::move(SDL_Color{ r,g,b,alpha }));
}

void  TextProperty::SetColor(SDL_Color color)
{
	m_PropertyTable->SetPropertyValue<SDL_Color>(FONT_COLOR, std::move(color));
}

SDL_Color TextProperty::GetColor() const
{
	return m_PropertyTable->GetPropertyValue<SDL_Color>(FONT_COLOR);
}