#include "stdafx.h"
#include <Property.h>
#include <string>
Property::Property() {}

Property::~Property() {}

int 	Property::getLayoutWidth() const
{
	return m_inner_position.w;
}


void 	Property::setLayoutWidth(int value)
{
	m_inner_position.w = value;
}


int 	Property::getLayoutHeight() const
{
	return m_inner_position.h;
}


void 	Property::setLayoutHeight(int value)
{
	m_inner_position.h = value;
}


int 	Property::getActualWidth() const
{
	return m_src_rect.w;
}


void 	Property::setActualWidth(int value)
{
	m_src_rect.w = value;
}



int 	Property::getActualHeight() const
{
	return m_src_rect.h;
}


void 	Property::setActualHeight(int value)
{
	m_src_rect.h = value;
}


bool 	Property::isVisible() const
{
	return m_bVisible;
}


void 	Property::setVisible(bool value)
{
	m_bVisible = value;
}


uint8_t 	Property::getOpacity() const
{
	return m_uAlpha;
}


void 	Property::setOpacity(uint8_t value)
{
	m_uAlpha = value;
}



void Property::SetLayoutPosition(int x, int y)
{
	m_inner_position.x = x;
	m_inner_position.y = y;
}

SDL_Point Property::GetLayoutPosition() const
{
	return{ m_inner_position.x , m_inner_position.y };
}

SDL_Rect Property::GetActualTransformation() const
{
	return m_src_rect;
}

SDL_Rect Property::GetLayoutTransformation() const
{
	return m_inner_position;
}

void Property::SetActualTransformation(SDL_Rect& r)
{
	m_src_rect = r;
}

void Property::SetLayoutTransformation(SDL_Rect& r)
{
	m_inner_position = r;
}

void  Property::SetLoadToGPU(bool isload)
{
	m_bLoadToGPU = isload;
}

bool  Property::IsLoadToGPU() const
{
	return m_bLoadToGPU;
}


inline	void Property::SetAngle(double angle)
{
	m_angle = angle;
}

double Property::GetAngle() const
{
	return m_angle;
}


void Property::SetCenterPoint(int x, int y)
{
	m_centerPoint = { x,y };
}

SDL_Point Property::GetCenterPoint() const
{
	return m_centerPoint;
}


void Property::SetFlip(SDL_RendererFlip flip)
{
	m_flip = flip;
}

SDL_RendererFlip Property::GetFlip() const
{
	return m_flip;
}

TextProperty::TextProperty() {}
TextProperty::~TextProperty() {}

void TextProperty::SetText(const char* text)
{
	m_text = std::string(text);
}

std::string TextProperty::GetText() const
{
	return m_text;
}


void TextProperty::SetFontName(const char* font_name)
{
	m_font_name = std::string(font_name);
}

std::string TextProperty::GetFontName() const
{
	return m_font_name;
}


void TextProperty::SetFontSize(int32_t psize)
{
	m_point_size = psize;
}

int32_t TextProperty::GetFontSize() const
{
	return m_point_size;
}


void TextProperty::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
{
	m_fore_ground_color = SDL_Color{ r,g,b,alpha };
}

SDL_Color TextProperty::GetColor() const
{
	return m_fore_ground_color;
}

NodeResourceProperty::NodeResourceProperty() {}
NodeResourceProperty::~NodeResourceProperty() {}

std::string NodeResourceProperty::GetResourceName() const
{
	return m_resource_name;
}

void NodeResourceProperty::SetResourceName(const char* str_resource)
{
	m_resource_name = std::string(str_resource);
}
