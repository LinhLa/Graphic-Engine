#include "stdafx.h"
#include <OriginProperty.h>
#include "PropertyDefine.h"
#include <string>
#include "utils.h"

const std::map<std::string, int> gMapAligment =
{
	{"center", ALGIN_CENTER},
	{"top", ALGIN_TOP},
	{"bottom", ALGIN_BOTTOM},
	{"left", ALGIN_LEFT},
	{"right", ALGIN_RIGHT}
};


OriginProperty::OriginProperty(PropertyTable* propertyTable):m_PropertyTable(propertyTable)
{
	if (!m_PropertyTable)
	{
		throw std::logic_error("invalid_argument");
	}
}

OriginProperty::~OriginProperty() {}

OriginProperty& OriginProperty::operator=(const OriginProperty& rhs)
{
	SetVisible(rhs.isVisible());
	SetOpacity(rhs.GetOpacity());
	SetClip(rhs.IsClip());
	SetAngle(rhs.GetAngle());
	SetCenterPoint(rhs.GetCenterPoint());
	SetBroadCastEvent(rhs.IsBroadCastEvent());
	SetFlip(rhs.GetFlip());
	return *this;
}

#ifdef OPENGL_RENDERING
bool OriginProperty::isValidColor(glm::vec4 color)
{
	auto sum = color.a + color.r + color.g + color.b;
	return (!util::almost_equal(sum, 0.0F, 3));
}
#else
bool OriginProperty::isValidColor(SDL_Color color)
{
	return (0 != (color.a + color.r + color.g + color.b));
}
#endif

void OriginProperty::init()
{
	m_PropertyTable->AddProperty(VISIBLE, Property<bool>::create(VISIBLE, true, BOOL));
	m_PropertyTable->AddProperty(IS_CLIP, Property<bool>::create(IS_CLIP, true, BOOL));
	m_PropertyTable->AddProperty(ANGLE, Property<float>::create(ANGLE, 0.0F, FLOAT));
	m_PropertyTable->AddProperty(IS_BROADCAST_EVENT, Property<bool>::create(IS_BROADCAST_EVENT, false, BOOL));
	m_PropertyTable->AddProperty(ALIGNMENT_VERIZONTAL, Property<int>::create(ALIGNMENT_VERIZONTAL, ALGIN_TOP, INT));
	m_PropertyTable->AddProperty(ALIGNMENT_HORIZONTAL, Property<int>::create(ALIGNMENT_HORIZONTAL, ALGIN_LEFT, INT));
#ifdef OPENGL_RENDERING
	m_PropertyTable->AddProperty(OPACITY, Property<float>::create(OPACITY, 1.0f, FLOAT));
	m_PropertyTable->AddProperty(CENTER_POINT, Property<glm::vec2>::create(CENTER_POINT, glm::vec2(0.0f), VEC2));
	m_PropertyTable->AddProperty(FORE_GROUND_COLOR, Property<glm::vec4>::create(FORE_GROUND_COLOR, glm::vec4(1.0F), VEC4_COLOR));
	m_PropertyTable->AddProperty(BACK_GROUND_COLOR, Property<glm::vec4>::create(BACK_GROUND_COLOR, glm::vec4(0.0F), VEC4_COLOR));
#else
	m_PropertyTable->AddProperty(OPACITY, Property<uint8_t>::create(OPACITY, 255, UINT8));
	m_PropertyTable->AddProperty(CENTER_POINT, Property<SDL_Point>::create(CENTER_POINT, {0,0}));
	m_PropertyTable->AddProperty(FORE_GROUND_COLOR, Property<SDL_Color>::create(FORE_GROUND_COLOR, SDL_Color{0,0,0,0}));
	m_PropertyTable->AddProperty(BACK_GROUND_COLOR, Property<SDL_Color>::create(BACK_GROUND_COLOR, SDL_Color{ 0,0,0,0 }));
#endif
	m_PropertyTable->AddProperty(PIVOT_POINT, Property<glm::vec3>::create(PIVOT_POINT, glm::vec3(0.0F), VEC3));
	m_PropertyTable->AddProperty(FLIP, Property<SDL_RendererFlip>::create(FLIP, SDL_FLIP_NONE));

	//<Set default value
	SetVisible(true);
	SetClip(false);
	SetAngle(0.0);
	SetBroadCastEvent(false);

	SetFlip(SDL_RendererFlip::SDL_FLIP_NONE);

#ifndef OPENGL_RENDERING
	SetOpacity(SDL_ALPHA_OPAQUE);
#else
	SetOpacity(1.0f);
#endif
}

bool OriginProperty::isVisible() const
{
	return m_PropertyTable->GetPropertyValue<bool>(VISIBLE);
}

void OriginProperty::SetVisible(bool value)
{
	m_PropertyTable->SetPropertyValue<bool>(VISIBLE, value);
}

void OriginProperty::SetClip(bool isClip)
{
	m_PropertyTable->SetPropertyValue<bool>(IS_CLIP, isClip);
}

bool OriginProperty::IsClip() const
{
	return m_PropertyTable->GetPropertyValue<bool>(IS_CLIP);
}

void OriginProperty::SetAngle(float angle)
{
	m_PropertyTable->SetPropertyValue<float>(ANGLE, angle);
}

float OriginProperty::GetAngle() const
{
	return m_PropertyTable->GetPropertyValue<float>(ANGLE);
}

void OriginProperty::SetBroadCastEvent(bool flag)
{
	m_PropertyTable->SetPropertyValue<bool>(IS_BROADCAST_EVENT, flag);
}

bool OriginProperty::IsBroadCastEvent() const
{
	return m_PropertyTable->GetPropertyValue<bool>(IS_BROADCAST_EVENT);
}

void OriginProperty::SetPivotPoint(glm::vec3 pivot)
{
	m_PropertyTable->SetPropertyValue<glm::vec3>(PIVOT_POINT, pivot);
}

glm::vec3 OriginProperty::GetPivotPoint() const
{
	return m_PropertyTable->GetPropertyValue<glm::vec3>(PIVOT_POINT);
}

void OriginProperty::SetFlip(SDL_RendererFlip flip)
{
	m_PropertyTable->SetPropertyValue<SDL_RendererFlip>(FLIP, flip);
}

SDL_RendererFlip OriginProperty::GetFlip() const
{
	return m_PropertyTable->GetPropertyValue<SDL_RendererFlip>(FLIP);
}

void OriginProperty::SetAlignHorizontal(int align)
{
	m_PropertyTable->SetPropertyValue<int>(ALIGNMENT_HORIZONTAL, align);
}

int OriginProperty::GetAlignHorizontal()
{
	return m_PropertyTable->GetPropertyValue<int>(ALIGNMENT_HORIZONTAL);
}

void OriginProperty::SetAlignVerizontal(int align)
{
	m_PropertyTable->SetPropertyValue<int>(ALIGNMENT_VERIZONTAL, align);
}

int OriginProperty::GetAlignVerizontal()
{
	return m_PropertyTable->GetPropertyValue<int>(ALIGNMENT_VERIZONTAL);
}

#ifndef OPENGL_RENDERING
uint8_t OriginProperty::GetOpacity() const
{
	return m_PropertyTable->GetPropertyValue<uint8_t>(OPACITY);
}

void OriginProperty::SetOpacity(uint8_t value)
{
	m_PropertyTable->SetPropertyValue<uint8_t>(OPACITY, value);
}

void OriginProperty::SetCenterPoint(int x, int y)
{
	SDL_Point centerPoint{ x,y };
	m_PropertyTable->SetPropertyValue<SDL_Point>(CENTER_POINT, centerPoint);
}

void OriginProperty::SetCenterPoint(SDL_Point point)
{
	m_PropertyTable->SetPropertyValue<SDL_Point>(CENTER_POINT, point);
}

SDL_Point OriginProperty::GetCenterPoint() const
{
	return m_PropertyTable->GetPropertyValue<SDL_Point>(CENTER_POINT);
}

void OriginProperty::SetForeGroundColor(SDL_Color color)
{
	return m_PropertyTable->SetPropertyValue<SDL_Color>(FORE_GROUND_COLOR, color);
}

SDL_Color OriginProperty::GetForeGroundColor() const
{
	return m_PropertyTable->GetPropertyValue<SDL_Color>(FORE_GROUND_COLOR);
}

void OriginProperty::SetBackGroundColor(SDL_Color color)
{
	return m_PropertyTable->SetPropertyValue<SDL_Color>(BACK_GROUND_COLOR, color);
}

SDL_Color OriginProperty::GetBackGroundColor() const
{
	return m_PropertyTable->GetPropertyValue<SDL_Color>(BACK_GROUND_COLOR);
}
#else
float OriginProperty::GetOpacity() const
{
	return m_PropertyTable->GetPropertyValue<float>(OPACITY);
}

void OriginProperty::SetOpacity(float value)
{
	m_PropertyTable->SetPropertyValue<float>(OPACITY, value);
}

void OriginProperty::SetCenterPoint(glm::vec2 point)
{
	m_PropertyTable->SetPropertyValue<glm::vec2>(CENTER_POINT, point);
}

glm::vec2 OriginProperty::GetCenterPoint() const
{
	return m_PropertyTable->GetPropertyValue<glm::vec2>(CENTER_POINT);
}

void OriginProperty::SetForeGroundColor(glm::vec4 color)
{
	return m_PropertyTable->SetPropertyValue<glm::vec4>(FORE_GROUND_COLOR, color);
}

glm::vec4 OriginProperty::GetForeGroundColor() const
{
	return m_PropertyTable->GetPropertyValue<glm::vec4>(FORE_GROUND_COLOR);
}

void OriginProperty::SetBackGroundColor(glm::vec4 color)
{
	return m_PropertyTable->SetPropertyValue<glm::vec4>(BACK_GROUND_COLOR, color);
}

glm::vec4 OriginProperty::GetBackGroundColor() const
{
	return m_PropertyTable->GetPropertyValue<glm::vec4>(BACK_GROUND_COLOR);
}
#endif

