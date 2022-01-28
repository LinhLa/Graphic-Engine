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


OriginProperty::OriginProperty(PropertyTable* pPropertyTable)
{
	if (!pPropertyTable)
	{
		_ASSERT(false);
	}
	m_pPropertyTable = pPropertyTable;
}

OriginProperty::~OriginProperty() {}

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
	m_pPropertyTable->AddProperty(VISIBLE, Property<bool>::create(VISIBLE, true, GL_BOOL));
	m_pPropertyTable->AddProperty(IS_CLIP, Property<bool>::create(IS_CLIP, false, GL_BOOL));
	m_pPropertyTable->AddProperty(ANGLE, Property<float>::create(ANGLE, 0.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(IS_BROADCAST_EVENT, Property<bool>::create(IS_BROADCAST_EVENT, false, GL_BOOL));
	m_pPropertyTable->AddProperty(ALIGNMENT_VERIZONTAL, Property<int>::create(ALIGNMENT_VERIZONTAL, ALGIN_TOP, GL_INT));
	m_pPropertyTable->AddProperty(ALIGNMENT_HORIZONTAL, Property<int>::create(ALIGNMENT_HORIZONTAL, ALGIN_LEFT, GL_INT));
#ifdef OPENGL_RENDERING
	m_pPropertyTable->AddProperty(OPACITY, Property<float>::create(OPACITY, 1.0f, GL_FLOAT));
	m_pPropertyTable->AddProperty(CENTER_POINT, Property<glm::vec2>::create(CENTER_POINT, glm::vec2(0.0f), GL_FLOAT_VEC2));
	m_pPropertyTable->AddProperty(FORE_GROUND_COLOR, Property<glm::vec4>::create(FORE_GROUND_COLOR, glm::vec4(1.0F), GL_FLOAT_VEC4));
	m_pPropertyTable->AddProperty(BACK_GROUND_COLOR, Property<glm::vec4>::create(BACK_GROUND_COLOR, glm::vec4(0.0F), GL_FLOAT_VEC4));
#else
	m_pPropertyTable->AddProperty(OPACITY, Property<uint8_t>::create(OPACITY, 255, UGL_INT8));
	m_pPropertyTable->AddProperty(CENTER_POGL_INT, Property<SDL_Point>::create(CENTER_POGL_INT, {0,0}));
	m_pPropertyTable->AddProperty(FORE_GROUND_COLOR, Property<SDL_Color>::create(FORE_GROUND_COLOR, SDL_Color{0,0,0,0}));
	m_pPropertyTable->AddProperty(BACK_GROUND_COLOR, Property<SDL_Color>::create(BACK_GROUND_COLOR, SDL_Color{ 0,0,0,0 }));
#endif
	m_pPropertyTable->AddProperty(PIVOT_POINT, Property<glm::vec3>::create(PIVOT_POINT, glm::vec3(0.0F), GL_FLOAT_VEC3));
	m_pPropertyTable->AddProperty(FLIP, Property<SDL_RendererFlip>::create(FLIP, SDL_FLIP_NONE));

#ifndef OPENGL_RENDERING
	SetOpacity(SDL_ALPHA_OPAQUE);
#else
	SetOpacity(1.0f);
#endif
}

bool OriginProperty::isVisible() const
{
	return m_pPropertyTable->GetPropertyValue<bool>(VISIBLE);
}

void OriginProperty::SetVisible(bool value)
{
	m_pPropertyTable->SetPropertyValue<bool>(VISIBLE, value);
}

void OriginProperty::SetClip(bool isClip)
{
	m_pPropertyTable->SetPropertyValue<bool>(IS_CLIP, isClip);
}

bool OriginProperty::IsClip() const
{
	return m_pPropertyTable->GetPropertyValue<bool>(IS_CLIP);
}

void OriginProperty::SetAngle(float angle)
{
	m_pPropertyTable->SetPropertyValue<float>(ANGLE, angle);
}

float OriginProperty::GetAngle() const
{
	return m_pPropertyTable->GetPropertyValue<float>(ANGLE);
}

void OriginProperty::SetBroadCastEvent(bool flag)
{
	m_pPropertyTable->SetPropertyValue<bool>(IS_BROADCAST_EVENT, flag);
}

bool OriginProperty::IsBroadCastEvent() const
{
	return m_pPropertyTable->GetPropertyValue<bool>(IS_BROADCAST_EVENT);
}

void OriginProperty::SetPivotPoint(glm::vec3 pivot)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(PIVOT_POINT, pivot);
}

glm::vec3 OriginProperty::GetPivotPoint() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(PIVOT_POINT);
}

void OriginProperty::SetFlip(SDL_RendererFlip flip)
{
	m_pPropertyTable->SetPropertyValue<SDL_RendererFlip>(FLIP, flip);
}

SDL_RendererFlip OriginProperty::GetFlip() const
{
	return m_pPropertyTable->GetPropertyValue<SDL_RendererFlip>(FLIP);
}

void OriginProperty::SetAlignHorizontal(int align)
{
	m_pPropertyTable->SetPropertyValue<int>(ALIGNMENT_HORIZONTAL, align);
}

int OriginProperty::GetAlignHorizontal()
{
	return m_pPropertyTable->GetPropertyValue<int>(ALIGNMENT_HORIZONTAL);
}

void OriginProperty::SetAlignVerizontal(int align)
{
	m_pPropertyTable->SetPropertyValue<int>(ALIGNMENT_VERIZONTAL, align);
}

int OriginProperty::GetAlignVerizontal()
{
	return m_pPropertyTable->GetPropertyValue<int>(ALIGNMENT_VERIZONTAL);
}

#ifndef OPENGL_RENDERING
uint8_t OriginProperty::GetOpacity() const
{
	return m_pPropertyTable->GetPropertyValue<uint8_t>(OPACITY);
}

void OriginProperty::SetOpacity(uint8_t value)
{
	m_pPropertyTable->SetPropertyValue<uint8_t>(OPACITY, value);
}

void OriginProperty::SetCenterPoint(int x, int y)
{
	SDL_Point centerPoint{ x,y };
	m_pPropertyTable->SetPropertyValue<SDL_Point>(CENTER_POGL_INT, centerPoint);
}

void OriginProperty::SetCenterPoint(SDL_Point point)
{
	m_pPropertyTable->SetPropertyValue<SDL_Point>(CENTER_POGL_INT, point);
}

SDL_Point OriginProperty::GetCenterPoint() const
{
	return m_pPropertyTable->GetPropertyValue<SDL_Point>(CENTER_POGL_INT);
}

void OriginProperty::SetForeGroundColor(SDL_Color color)
{
	return m_pPropertyTable->SetPropertyValue<SDL_Color>(FORE_GROUND_COLOR, color);
}

SDL_Color OriginProperty::GetForeGroundColor() const
{
	return m_pPropertyTable->GetPropertyValue<SDL_Color>(FORE_GROUND_COLOR);
}

void OriginProperty::SetBackGroundColor(SDL_Color color)
{
	return m_pPropertyTable->SetPropertyValue<SDL_Color>(BACK_GROUND_COLOR, color);
}

SDL_Color OriginProperty::GetBackGroundColor() const
{
	return m_pPropertyTable->GetPropertyValue<SDL_Color>(BACK_GROUND_COLOR);
}
#else
float OriginProperty::GetOpacity() const
{
	return m_pPropertyTable->GetPropertyValue<float>(OPACITY);
}

void OriginProperty::SetOpacity(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(OPACITY, value);
}

void OriginProperty::SetCenterPoint(glm::vec2 point)
{
	m_pPropertyTable->SetPropertyValue<glm::vec2>(CENTER_POINT, point);
}

glm::vec2 OriginProperty::GetCenterPoint() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec2>(CENTER_POINT);
}

void OriginProperty::SetForeGroundColor(glm::vec4 color)
{
	return m_pPropertyTable->SetPropertyValue<glm::vec4>(FORE_GROUND_COLOR, color);
}

glm::vec4 OriginProperty::GetForeGroundColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(FORE_GROUND_COLOR);
}

void OriginProperty::SetBackGroundColor(glm::vec4 color)
{
	return m_pPropertyTable->SetPropertyValue<glm::vec4>(BACK_GROUND_COLOR, color);
}

glm::vec4 OriginProperty::GetBackGroundColor() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec4>(BACK_GROUND_COLOR);
}
#endif

