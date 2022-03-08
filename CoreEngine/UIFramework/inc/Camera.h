#pragma once
#include "creator.h"
#include "CameraProperty.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    CAM_FORWARD,
    CAM_BACKWARD,
    CAM_LEFT,
    CAM_RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera final : public creator<Camera>
{
private:
        glm::mat4 m_view = glm::mat4(1.0f);

        glm::vec3 m_Pos = glm::vec3(0.0f);
        glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_Target = glm::vec3(0.0f);

        float m_Near = 0.1F;
        float m_Far = 100.0F;
        float m_Yaw = YAW;
        float m_Pitch = PITCH;
        float m_Zoom = ZOOM;
        float m_Speed = SPEED;
        float m_Sensitivity = SENSITIVITY;

        uint8_t m_CamType = ORTHOGRAPHIC;
protected:
        friend class creator<Camera>;
        Camera(CameraPropertyPtr);
public:
        ~Camera();

        float NearPlane() const;
        float FarPlane() const;
        glm::mat4 View();
        float Zoom() const;
        float Sensitivity() const;
        float Speed() const;
        uint8_t CamType() const;
        glm::mat4 projectionMatrix();
        void demoRotateAround();
};

typedef std::shared_ptr<Camera> CameraPtr;