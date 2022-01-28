#include "stdafx.h"
#include <string>
#include <LayoutProperty.h>
#include "PropertyDefine.h"
#include "UIHelper.h"
LayoutProperty::LayoutProperty(PropertyTable* pPropertyTable)
{
	if (!pPropertyTable)
	{
		_ASSERT(false);
	}
	m_pPropertyTable = pPropertyTable;
}

LayoutProperty::~LayoutProperty() 
{}

void LayoutProperty::init()
{
#ifdef OPENGL_RENDERING
	m_pPropertyTable->AddProperty(X_COORDINATOR, Property<float>::create(X_COORDINATOR, 0.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(Y_COORDINATOR, Property<float>::create(Y_COORDINATOR, 0.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(SCALE_X, Property<float>::create(SCALE_X, 1.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(SCALE_Y, Property<float>::create(SCALE_Y, 1.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(TRANS_VECTOR, Property<glm::vec3>::create(TRANS_VECTOR, glm::vec3(0.0F), GL_FLOAT_VEC3));
	m_pPropertyTable->AddProperty(ROTATE_VECTOR, Property<glm::vec3>::create(ROTATE_VECTOR, glm::vec3(0.0F), GL_FLOAT_VEC3));
	m_pPropertyTable->AddProperty(SCALE_VECTOR, Property<glm::vec3>::create(SCALE_VECTOR, glm::vec3(1.0F), GL_FLOAT_VEC3));
	m_pPropertyTable->AddProperty(LAYOUT_WIDTH, Property<float>::create(LAYOUT_WIDTH, 0.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(LAYOUT_HEIGHT, Property<float>::create(LAYOUT_HEIGHT, 0.0F, GL_FLOAT));

	std::function<void(float&&)> onMoveX = std::bind(&LayoutProperty::onMoveX, this, std::placeholders::_1);
	std::function<void(float&&)> onMoveY = std::bind(&LayoutProperty::onMoveY, this, std::placeholders::_1);
	std::function<void(float&&)> onScaleX = std::bind(&LayoutProperty::onScaleX, this, std::placeholders::_1);
	std::function<void(float&&)> onScaleY = std::bind(&LayoutProperty::onScaleY, this, std::placeholders::_1);

	m_pPropertyTable->BindPropertySignal(X_COORDINATOR, onMoveX);
	m_pPropertyTable->BindPropertySignal(Y_COORDINATOR, onMoveY);
	m_pPropertyTable->BindPropertySignal(SCALE_X, onScaleX);
	m_pPropertyTable->BindPropertySignal(SCALE_Y, onScaleY);

#else
	m_pPropertyTable->AddProperty(X_COORDINATOR, Property<int>::create(X_COORDINATOR, 0.0F, INT));
	m_pPropertyTable->AddProperty(Y_COORDINATOR, Property<int>::create(Y_COORDINATOR, 0.0F, INT));
	m_pPropertyTable->AddProperty(SCALE_X, Property<float>::create(SCALE_X, 1.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(SCALE_Y, Property<float>::create(SCALE_Y, 1.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(LAYOUT_WIDTH, Property<int>::create(LAYOUT_WIDTH, 0, INT));
	m_pPropertyTable->AddProperty(LAYOUT_HEIGHT, Property<int>::create(LAYOUT_HEIGHT, 0, INT));
#endif

	//<Set default value
	SDL_Rect window_rect = UIHelper::GetWindowRect();
#ifdef OPENGL_RENDERING
	SetLayoutSize(glm::vec2(window_rect.w, window_rect.h));
#else
	SetLayoutInformation(window_rect);
#endif
}

#ifdef OPENGL_RENDERING
void LayoutProperty::onMoveX(float&& x)
{
	auto transform = GetLayoutTransform();
	transform.x = x;
	SetLayoutTransform(transform);
}

void LayoutProperty::onMoveY(float&& y)
{
	auto transform = GetLayoutTransform();
	transform.y = y;
	SetLayoutTransform(transform);
}

void LayoutProperty::onScaleX(float&& x)
{
	auto scale = GetLayoutScale();
	scale.x = x;
	SetLayoutScale(scale);
}

void LayoutProperty::onScaleY(float&& y)
{
	auto scale = GetLayoutScale();
	scale.y = y;
	SetLayoutScale(scale);
}

void LayoutProperty::SetLayoutSize(glm::vec2 size)
{
	m_pPropertyTable->SetPropertyValue<float>(LAYOUT_WIDTH, std::move(size.x));
	m_pPropertyTable->SetPropertyValue<float>(LAYOUT_HEIGHT, std::move(size.y));
}

glm::vec2 LayoutProperty::GetLayoutSize() const
{
	glm::vec2 size;
	size.x = m_pPropertyTable->GetPropertyValue<float>(LAYOUT_WIDTH);
	size.y = m_pPropertyTable->GetPropertyValue<float>(LAYOUT_HEIGHT);
	return size;
}


void LayoutProperty::SetLayoutTransform(glm::vec3 vTrans)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(TRANS_VECTOR, std::move(vTrans));
}

glm::vec3 LayoutProperty::GetLayoutTransform() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(TRANS_VECTOR);
}

void LayoutProperty::SetRotation(glm::vec3 vRotate)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(ROTATE_VECTOR, std::move(vRotate));
}

glm::vec3 LayoutProperty::GetRotation() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(ROTATE_VECTOR);
}

void LayoutProperty::SetLayoutScale(glm::vec3 vScale)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(SCALE_VECTOR, std::move(vScale));
}

glm::vec3 LayoutProperty::GetLayoutScale() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(SCALE_VECTOR);
}
#else

void LayoutProperty::SetLayoutScaleX(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(SCALE_X, std::move(value));
}

float LayoutProperty::GetLayoutScaleX() const
{
	return m_pPropertyTable->GetPropertyValue<float>(SCALE_X);
}

void LayoutProperty::SetLayoutScaleY(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(SCALE_Y, std::move(value));
}

float LayoutProperty::GetLayoutScaleY() const
{
	return m_pPropertyTable->GetPropertyValue<float>(SCALE_Y);
}

int LayoutProperty::GetLayoutWidth() const
{
	return m_pPropertyTable->GetPropertyValue<int>(LAYOUT_WIDTH);
}

void LayoutProperty::SetLayoutWidth(int value)
{
	m_pPropertyTable->SetPropertyValue<int>(LAYOUT_WIDTH, std::move(value));
}

int LayoutProperty::GetLayoutHeight() const
{
	return m_pPropertyTable->GetPropertyValue<int>(LAYOUT_HEIGHT);
}

void LayoutProperty::SetLayoutHeight(int value)
{
	m_pPropertyTable->SetPropertyValue<int>(LAYOUT_HEIGHT, std::move(value));
}

void LayoutProperty::SetLayoutPosition(int x, int y)
{
	m_pPropertyTable->SetPropertyValue<int>(X_COORDINATOR, std::move(x));
	m_pPropertyTable->SetPropertyValue<int>(Y_COORDINATOR, std::move(y));
}

SDL_Point LayoutProperty::GetLayoutPosition() const
{
	SDL_Rect display_rect = GetLayoutInformation();
	return { display_rect.x , display_rect.y };
}

SDL_Rect LayoutProperty::GetLayoutInformation() const
{
	SDL_Rect display_rect;
	display_rect.x = m_pPropertyTable->GetPropertyValue<int>(X_COORDINATOR);
	display_rect.y = m_pPropertyTable->GetPropertyValue<int>(Y_COORDINATOR);
	display_rect.w = m_pPropertyTable->GetPropertyValue<int>(LAYOUT_WIDTH);
	display_rect.h = m_pPropertyTable->GetPropertyValue<int>(LAYOUT_HEIGHT);
	return display_rect;
}

void LayoutProperty::SetLayoutInformation(const SDL_Rect& r)
{
	SetLayoutPosition(r.x, r.y);
	SetLayoutHeight(r.h);
	SetLayoutWidth(r.w);
}
#endif
