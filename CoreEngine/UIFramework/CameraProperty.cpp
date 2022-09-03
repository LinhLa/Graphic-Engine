#include "stdafx.h"
#include <string>
#include <CameraProperty.h>
#include "PropertyDefine.h"

const std::map<std::string, CAMERA_TYPE_ENUM> cameraMap =
{
	{"ortho", ORTHOGRAPHIC},
	{"perspective", PERSPECTIVE}
};

CameraProperty::CameraProperty(PropertyTable* pPropertyTable)
{
	if (!pPropertyTable)
	{
		_ASSERT(false);
	}
	m_pPropertyTable = pPropertyTable;
}

CameraProperty::~CameraProperty() {}

void CameraProperty::init()
{
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
}

glm::vec3 CameraProperty::GetCamPos() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(CAM_POSITION);
}

void CameraProperty::SetCamPos(glm::vec3 value)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(CAM_POSITION, std::move(value));
}

glm::vec3 CameraProperty::GetCamFront() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(CAM_FRONT);
}

void CameraProperty::SetCamFront(glm::vec3 value)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(CAM_FRONT, std::move(value));
}

glm::vec3 CameraProperty::GetCamTarget() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(CAM_TARGET);
}

void CameraProperty::SetCamTarget(glm::vec3 value)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(CAM_TARGET, std::move(value));
}

glm::vec3 CameraProperty::GetCamUp() const
{
	return m_pPropertyTable->GetPropertyValue<glm::vec3>(CAM_UP);
}

void CameraProperty::SetCamUp(glm::vec3 value)
{
	m_pPropertyTable->SetPropertyValue<glm::vec3>(CAM_UP, std::move(value));
}

float CameraProperty::GetCamPitch() const
{
	return m_pPropertyTable->GetPropertyValue<float>(CAM_PITCH);
}

void CameraProperty::SetCamPitch(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(CAM_PITCH, std::move(value));
}

float CameraProperty::GetCamYaw() const
{
	return m_pPropertyTable->GetPropertyValue<float>(CAM_YAW);
}

void CameraProperty::SetCamYaw(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(CAM_YAW, std::move(value));
}

float CameraProperty::GetCamSpeed() const
{
	return m_pPropertyTable->GetPropertyValue<float>(CAM_SPEED);
}

void CameraProperty::SetCamSpeed(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(CAM_SPEED, std::move(value));
}

float CameraProperty::GetFOV() const
{
	return m_pPropertyTable->GetPropertyValue<float>(FOV);
}

void CameraProperty::SetFOV(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(FOV, std::move(value));
}

float CameraProperty::GetNearPlane() const
{
	return m_pPropertyTable->GetPropertyValue<float>(NEAR_PLANE);
}

void CameraProperty::SetNearPlane(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(NEAR_PLANE, std::move(value));
}

float CameraProperty::GetFarPlane() const
{
	return m_pPropertyTable->GetPropertyValue<float>(FAR_PLANE);
}

void CameraProperty::SetFarPlane(float value)
{
	m_pPropertyTable->SetPropertyValue<float>(FAR_PLANE, std::move(value));
}

int CameraProperty::GetCameraType() const
{
	return m_pPropertyTable->GetPropertyValue<int>(CAMERA_TYPE);
}

void CameraProperty::SetCameraType(int value)
{
	m_pPropertyTable->SetPropertyValue<int>(CAMERA_TYPE, std::move(value));
}