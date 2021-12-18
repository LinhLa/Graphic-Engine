#pragma once
#include "stdafx.h"
#include "Camera.h"
#include <SDL.h>
#include <chrono>
#include "WindowRender.h"

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
    m_CamType   = glProperty->GetCameraType();
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
    m_view = glm::lookAt(m_Pos , m_Target, m_Up);
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

uint8_t Camera::CamType() const
{
    return m_CamType;
}

glm::mat4 Camera::projectionMatrix()
{
    glm::mat4 ProjectionMatrix = glm::mat4();
    float width = static_cast<float>(WindowRender::GetInstance()->getWidth());
    float height = static_cast<float>(WindowRender::GetInstance()->getHeight());
    if (ORTHOGRAPHIC == m_CamType)
    {
        ProjectionMatrix = glm::ortho(0.0f, width, 0.0f, height, m_Near, m_Far);
    }
    else if (PERSPECTIVE == m_CamType)
    {
        ProjectionMatrix = glm::perspective(glm::radians(m_Zoom), width / height, m_Near, m_Far);
    }
    else {}

    return ProjectionMatrix;
}

void Camera::demoRotateAround()
{
    const float radius = 10.0f;
    m_Pos.x = static_cast<float>(sin((SDL_GetTicks() / CLOCKS_PER_SEC)) * radius);
    m_Pos.z = static_cast<float>(cos((SDL_GetTicks() / CLOCKS_PER_SEC)) * radius);
}