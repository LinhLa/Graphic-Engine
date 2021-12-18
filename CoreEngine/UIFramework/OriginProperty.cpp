#include "stdafx.h"
#include <OriginProperty.h>
#include "PropertyDefine.h"
#include <string>

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

void OriginProperty::init()
{
	m_PropertyTable->AddProperty(VISIBLE, Property<bool>::create(VISIBLE, true, BOOL));
	m_PropertyTable->AddProperty(IS_CLIP, Property<bool>::create(IS_CLIP, true, BOOL));
	m_PropertyTable->AddProperty(ANGLE, Property<float>::create(ANGLE, 0.0F, FLOAT));

	m_PropertyTable->AddProperty(IS_BROADCAST_EVENT, Property<bool>::create(IS_BROADCAST_EVENT));

	m_PropertyTable->AddProperty(OPACITY, Property<uint8_t>::create(OPACITY, 255, UINT8));
	m_PropertyTable->AddProperty(CENTER_POINT, Property<SDL_Point>::create(CENTER_POINT));
	m_PropertyTable->AddProperty(PIVOT_POINT, Property<glm::vec3>::create(PIVOT_POINT, glm::vec3(0.0F), VEC3));
	m_PropertyTable->AddProperty(FLIP, Property<SDL_RendererFlip>::create(FLIP));

	m_PropertyTable->AddProperty(FORE_GROUND_COLOR, Property<SDL_Color>::create(FORE_GROUND_COLOR));
	m_PropertyTable->AddProperty(BACK_GROUND_COLOR, Property<SDL_Color>::create(BACK_GROUND_COLOR));

	//<Set default value
	SetVisible(true);
	SetClip(false);
	SetAngle(0.0);
	SetBroadCastEvent(false);

	SetOpacity(SDL_ALPHA_OPAQUE);
	SetFlip(SDL_RendererFlip::SDL_FLIP_NONE);
	SetForeGroundColor(SDL_Color{0,0,0,0});
	SetBackGroundColor(SDL_Color{0,0,0,0});
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

void OriginProperty::SetAngle(double angle)
{
	m_PropertyTable->SetPropertyValue<float>(ANGLE, angle);
}

double OriginProperty::GetAngle() const
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