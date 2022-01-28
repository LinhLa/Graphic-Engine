#include "stdafx.h"
#include <string>
#include <GLProperty.h>
#include "PropertyDefine.h"
#include "UIHelper.h"
#include "Configuration.h"

const std::map<std::string, CAMERA_TYPE_ENUM> cameraMap =
{
	{"ortho", ORTHOGRAPHIC},
	{"perspective", PERSPECTIVE}
};

GLProperty::GLProperty(PropertyTable* pPropertyTable)
{
	if (!pPropertyTable)
	{
		_ASSERT(false);
	}
	m_pPropertyTable = pPropertyTable;
}

GLProperty::~GLProperty() {}

void GLProperty::init()
{
	m_pPropertyTable->AddProperty(BLEND_INDENSITY, Property<float>::create(BLEND_INDENSITY, 1.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(CAM_POSITION, Property<glm::vec3>::create(CAM_POSITION, glm::vec3(0.0F, 0.0F, 3.0F), GL_FLOAT_VEC3));
	m_pPropertyTable->AddProperty(CAM_TARGET, Property<glm::vec3>::create(CAM_TARGET, glm::vec3(0.0F), GL_FLOAT_VEC3));
	m_pPropertyTable->AddProperty(CAM_FRONT, Property<glm::vec3>::create(CAM_FRONT, glm::vec3(0.0F, 0.0F, -1.0F), GL_FLOAT_VEC3));
	m_pPropertyTable->AddProperty(CAM_UP, Property<glm::vec3>::create(CAM_UP, glm::vec3(0.0F, 1.0F, 0.0F), GL_FLOAT_VEC3));
	m_pPropertyTable->AddProperty(CAM_PITCH, Property<float>::create(CAM_PITCH, 0.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(CAM_YAW, Property<float>::create(CAM_YAW, 0.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(CAM_SPEED, Property<float>::create(CAM_SPEED, 2.5F, GL_FLOAT));
	m_pPropertyTable->AddProperty(FOV, Property<float>::create(FOV, 45.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(NEAR_PLANE, Property<float>::create(NEAR_PLANE, 0.1F, GL_FLOAT));
	m_pPropertyTable->AddProperty(FAR_PLANE, Property<float>::create(FAR_PLANE, 100.0F, GL_FLOAT));
	m_pPropertyTable->AddProperty(CAMERA_TYPE, Property<int>::create(CAMERA_TYPE, ORTHOGRAPHIC, GL_INT));
	m_pPropertyTable->AddProperty(VIEW_PORT, Property<glm::i32vec4>::create(VIEW_PORT, glm::i32vec4(0,0,Configuration::GetInstance()->width,Configuration::GetInstance()->height),GL_INT_VEC4));
}

float GLProperty::GetBlendIndensity() const
{
	return m_pPropertyTable->GetPropertyValue<float>(BLEND_INDENSITY);
}

void GLProperty::SetBlendIndensity(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(BLEND_INDENSITY, std::move(value));
}

glm::vec3 GLProperty::GetCamPos() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(CAM_POSITION);
}

void GLProperty::SetCamPos(glm::vec3 value)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(CAM_POSITION, std::move(value));
}

glm::vec3 GLProperty::GetCamFront() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(CAM_FRONT);
}

void GLProperty::SetCamFront(glm::vec3 value)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(CAM_FRONT, std::move(value));
}

glm::vec3 GLProperty::GetCamTarget() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(CAM_TARGET);
}

void GLProperty::SetCamTarget(glm::vec3 value)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(CAM_TARGET, std::move(value));
}

glm::vec3 GLProperty::GetCamUp() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(CAM_UP);
}

void GLProperty::SetCamUp(glm::vec3 value)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(CAM_UP, std::move(value));
}

float GLProperty::GetCamPitch() const
{
	return m_pPropertyTable->GetPropertyValue<float>(CAM_PITCH);
}

void GLProperty::SetCamPitch(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(CAM_PITCH, std::move(value));
}

float GLProperty::GetCamYaw() const
{
	return m_pPropertyTable->GetPropertyValue<float>(CAM_YAW);
}

void GLProperty::SetCamYaw(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(CAM_YAW, std::move(value));
}

float GLProperty::GetCamSpeed() const
{
	return m_pPropertyTable->GetPropertyValue<float>(CAM_SPEED);
}

void GLProperty::SetCamSpeed(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(CAM_SPEED, std::move(value));
}

float GLProperty::GetFOV() const
{
	return m_pPropertyTable->GetPropertyValue<float>(FOV);
}

void GLProperty::SetFOV(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(FOV, std::move(value));
}

float GLProperty::GetNearPlane() const
{
	return m_pPropertyTable->GetPropertyValue<float>(NEAR_PLANE);
}

void GLProperty::SetNearPlane(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(NEAR_PLANE, std::move(value));
}

float GLProperty::GetFarPlane() const
{
	return m_pPropertyTable->GetPropertyValue<float>(FAR_PLANE);
}

void GLProperty::SetFarPlane(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(FAR_PLANE, std::move(value));
}

int GLProperty::GetCameraType() const
{
	return m_pPropertyTable->GetPropertyValue<int>(CAMERA_TYPE);
}

void GLProperty::SetCameraType(int value)
{
	m_pPropertyTable->SetPropertyValue<int>(CAMERA_TYPE, std::move(value));
}

glm::i32vec4 GLProperty::GetViewPort() const
{
	return m_pPropertyTable->GetPropertyValue<glm::i32vec4>(VIEW_PORT);
}

void GLProperty::SetViewPort(glm::i32vec4 value)
{
	m_pPropertyTable->SetPropertyValue<glm::i32vec4>(VIEW_PORT, std::move(value));
}