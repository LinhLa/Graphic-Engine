#pragma once
#include "stdafx.h"
#include "Camera.h"
#include <SDL.h>
#include <chrono>

Camera::Camera(GLPropertyPtr glProperty)
{
    if (!glProperty)
    {
        throw std::logic_error("invalid argument");
    }
    m_Pos       = glProperty->GetCamPos();
    m_Front     = glProperty->GetCamFront();
    m_Up        = glProperty->GetCamUp();
    m_Target    = glProperty->GetCamTarget();
    m_Yaw       = glProperty->GetCamYaw();
    m_Pitch     = glProperty->GetCamPitch();
    m_Zoom      = glProperty->GetFOV();
    m_Speed     = glProperty->GetCamSpeed();

    m_Near      = glProperty->GetNearPlane();
    m_Far       = glProperty->GetFarPlane();
}

Camera::~Camera(){}

float Camera::NearPlane() const
{
    return m_Near;
}

float Camera::FarPlane() const
{
    return m_Far;
}

glm::mat4 Camera::View()
{
    //Calculate view matrix
    m_view = glm::lookAt(m_Pos , m_Pos + m_Front, m_Up);
    return m_view;
}

float Camera::Zoom() const
{
    return m_Zoom;
}

float Camera::Sensitivity() const
{
    return m_Sensitivity;
}

float Camera::Speed() const
{
    return m_Speed;
}

void Camera::demoRotateAround()
{
    const float radius = 10.0f;
    m_Pos.x = static_cast<float>(sin((SDL_GetTicks() / CLOCKS_PER_SEC)) * radius);
    m_Pos.z = static_cast<float>(cos((SDL_GetTicks() / CLOCKS_PER_SEC)) * radius);
}