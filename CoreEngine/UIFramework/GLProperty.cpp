#include "stdafx.h"
#include <string>
#include <GLProperty.h>
#include "PropertyDefine.h"
#include "UIHelper.h"

const std::map<std::string, CAMERA_TYPE_ENUM> cameraMap =
{
	{"ortho", ORTHOGRAPHIC},
	{"perspective", PERSPECTIVE}
};

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
	m_PropertyTable->AddProperty(CAM_POSITION, Property<glm::vec3>::create(CAM_POSITION));
	m_PropertyTable->AddProperty(CAM_TARGET, Property<glm::vec3>::create(CAM_TARGET));
	m_PropertyTable->AddProperty(CAM_FRONT, Property<glm::vec3>::create(CAM_FRONT));
	m_PropertyTable->AddProperty(CAM_UP, Property<glm::vec3>::create(CAM_UP));
	m_PropertyTable->AddProperty(CAM_PITCH, Property<float>::create(CAM_PITCH));
	m_PropertyTable->AddProperty(CAM_YAW, Property<float>::create(CAM_YAW));
	m_PropertyTable->AddProperty(CAM_YAW, Property<float>::create(CAM_SPEED));
	m_PropertyTable->AddProperty(FOV, Property<float>::create(FOV));
	m_PropertyTable->AddProperty(NEAR_PLANE, Property<float>::create(NEAR_PLANE));
	m_PropertyTable->AddProperty(FAR_PLANE, Property<float>::create(FAR_PLANE));
	m_PropertyTable->AddProperty(CAMERA_TYPE, Property<uint8_t>::create(CAMERA_TYPE));

	//<Set default value
	m_PropertyTable->SetPropertyValue<float>(BLEND_INDENSITY, std::move(1.0F));
	m_PropertyTable->SetPropertyValue<glm::vec3>(CAM_POSITION, std::move(glm::vec3(0.0F, 0.0F, 3.0F)));
	m_PropertyTable->SetPropertyValue<glm::vec3>(CAM_FRONT, std::move(glm::vec3(0.0F, 0.0F, -1.0F)));
	m_PropertyTable->SetPropertyValue<glm::vec3>(CAM_TARGET, std::move(glm::vec3(0.0F)));
	m_PropertyTable->SetPropertyValue<glm::vec3>(CAM_UP, std::move(glm::vec3(0.0F, 0.1F, 0.0F)));
	m_PropertyTable->SetPropertyValue<float>(CAM_PITCH, std::move(0.0F));
	m_PropertyTable->SetPropertyValue<float>(CAM_YAW, std::move(-90.0F));
	m_PropertyTable->SetPropertyValue<float>(FOV, std::move(45.0F));
	m_PropertyTable->SetPropertyValue<float>(NEAR_PLANE, std::move(0.1F));
	m_PropertyTable->SetPropertyValue<float>(FAR_PLANE, std::move(100.0F));
	m_PropertyTable->SetPropertyValue<float>(CAM_SPEED, std::move(2.5F));
	m_PropertyTable->SetPropertyValue<uint8_t>(CAMERA_TYPE, std::move(ORTHOGRAPHIC));
}

float GLProperty::GetBlendIndensity() const
{
	return m_PropertyTable->GetPropertyValue<float>(BLEND_INDENSITY);
}

void GLProperty::SetBlendIndensity(float value)
{
	m_PropertyTable->SetPropertyValue<float>(BLEND_INDENSITY, std::move(value));
}

glm::vec3 GLProperty::GetCamPos() const
{
	return m_PropertyTable->GetPropertyValue<glm::vec3>(CAM_POSITION);
}

void GLProperty::SetCamPos(glm::vec3 value)
{
	m_PropertyTable->SetPropertyValue<glm::vec3>(CAM_POSITION, std::move(value));
}

glm::vec3 GLProperty::GetCamFront() const
{
	return m_PropertyTable->GetPropertyValue<glm::vec3>(CAM_FRONT);
}

void GLProperty::SetCamFront(glm::vec3 value)
{
	m_PropertyTable->SetPropertyValue<glm::vec3>(CAM_FRONT, std::move(value));
}

glm::vec3 GLProperty::GetCamTarget() const
{
	return m_PropertyTable->GetPropertyValue<glm::vec3>(CAM_TARGET);
}

void GLProperty::SetCamTarget(glm::vec3 value)
{
	m_PropertyTable->SetPropertyValue<glm::vec3>(CAM_TARGET, std::move(value));
}

glm::vec3 GLProperty::GetCamUp() const
{
	return m_PropertyTable->GetPropertyValue<glm::vec3>(CAM_UP);
}

void GLProperty::SetCamUp(glm::vec3 value)
{
	m_PropertyTable->SetPropertyValue<glm::vec3>(CAM_UP, std::move(value));
}

float GLProperty::GetCamPitch() const
{
	return m_PropertyTable->GetPropertyValue<float>(CAM_PITCH);
}

void GLProperty::SetCamPitch(float value)
{
	m_PropertyTable->SetPropertyValue<float>(CAM_PITCH, std::move(value));
}

float GLProperty::GetCamYaw() const
{
	return m_PropertyTable->GetPropertyValue<float>(CAM_YAW);
}

void GLProperty::SetCamYaw(float value)
{
	m_PropertyTable->SetPropertyValue<float>(CAM_YAW, std::move(value));
}

float GLProperty::GetCamSpeed() const
{
	return m_PropertyTable->GetPropertyValue<float>(CAM_SPEED);
}

void GLProperty::SetCamSpeed(float value)
{
	m_PropertyTable->SetPropertyValue<float>(CAM_SPEED, std::move(value));
}

float GLProperty::GetFOV() const
{
	return m_PropertyTable->GetPropertyValue<float>(FOV);
}

void GLProperty::SetFOV(float value)
{
	m_PropertyTable->SetPropertyValue<float>(FOV, std::move(value));
}

float GLProperty::GetNearPlane() const
{
	return m_PropertyTable->GetPropertyValue<float>(NEAR_PLANE);
}

void GLProperty::SetNearPlane(float value)
{
	m_PropertyTable->SetPropertyValue<float>(NEAR_PLANE, std::move(value));
}

float GLProperty::GetFarPlane() const
{
	return m_PropertyTable->GetPropertyValue<float>(FAR_PLANE);
}

void GLProperty::SetFarPlane(float value)
{
	m_PropertyTable->SetPropertyValue<float>(FAR_PLANE, std::move(value));
}

uint8_t GLProperty::GetCameraType() const
{
	return m_PropertyTable->GetPropertyValue<uint8_t>(CAMERA_TYPE);
}

void GLProperty::SetCameraType(uint8_t value)
{
	m_PropertyTable->SetPropertyValue<uint8_t>(CAMERA_TYPE, std::move(value));
}