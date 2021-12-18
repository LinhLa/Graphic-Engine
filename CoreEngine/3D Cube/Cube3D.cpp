#include "stdafx.h"
#include <chrono>
#include "Cube3D.h"

#include "Scene.h"
#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "GLProperty.h"

#include "PropertyDefine.h"

#include "deltatime.h"
#include "UIHelper.h"
#include "SignalDefine.h"

#include "AnimationTimeLine.h"
#include "time_suffix.h"

#include "ImGuiShader.h"
#include "Library.h"
#include "Material.h"

#define CLOCKS_PER_MILISECOND (CLOCKS_PER_SEC / 1000)
using namespace UIHelper;

Cube3D::Cube3D(){}
Cube3D::~Cube3D() {}

void Cube3D::onKeyInputEvent(SDL_Event &&arg)
{
	//LOG_DEBUG("");
	if (SDL_MOUSEWHEEL == arg.type)
	{
		if (0 != arg.wheel.y) // scroll up/down
		{
			onMouseScroll(static_cast<float>(arg.wheel.y));
		}
		else
		{}
	}
	else if (SDL_MOUSEMOTION == arg.type)
	{
		//onMouseMove();
	}
	else if (SDL_MOUSEBUTTONDOWN == arg.type)
	{
	}
	else if (SDL_KEYDOWN == arg.type)
	{
		onKeyPress();
	}

	auto pMaterial = Library::GetInstance()->get<Material>(CUBE_3D_URL);
	ImGuiShader::GetInstance()->setProperty(pMaterial);
	ImGuiShader::GetInstance()->setProperty(m_pCube3D);
}
void Cube3D::onKeyPress()
{
	const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);
	float cameraSpeed = 0.01f * static_cast<float>(DeltaTime::GetInstance()->getDelta());
	auto glProperty		= m_pCube3D->GetPropertyMethodObj<GLProperty>();
	auto cameraPos		= glProperty->GetCamPos();
	auto cameraTarget	= glProperty->GetCamTarget();
	auto cameraUp		= glProperty->GetCamUp();
	auto cameraFront	= glProperty->GetCamFront();
	
	bool bLookAtMe = false;

	if (currentKeyStates[SDL_SCANCODE_W])
	{
		cameraPos += cameraSpeed * cameraFront;
	}

	if (currentKeyStates[SDL_SCANCODE_S])
	{
		cameraPos -= cameraSpeed * cameraFront;
	}

	if (currentKeyStates[SDL_SCANCODE_A])
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (currentKeyStates[SDL_SCANCODE_D])
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	float radius = glm::distance(cameraPos, glm::vec3(0.0f));
	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		bLookAtMe = true;
		cameraPos.x = -cos(45.0F) * radius;
		cameraPos.z = -sin(45.0F) * radius;
	}

	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		bLookAtMe = true;
		cameraPos.x = cos(45.0F) * radius;
		cameraPos.z = sin(45.0F) * radius;
	}

	if (currentKeyStates[SDL_SCANCODE_UP])
	{
		bLookAtMe = true;
		cameraPos.y = sin(45.0F) * radius;
		cameraPos.z = cos(-45.0F) * radius;
	}

	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		bLookAtMe = true;
		cameraPos.y = sin(45.0F) * radius;
		cameraPos.z = cos(45.0F) * radius;
	}

	glProperty->SetCamPos(cameraPos);

	if (bLookAtMe)
	{
		lookAtMe();
	}
}

void Cube3D::onMouseMove()
{
	//Get mouse position
	static bool firstMouse = true;
	static int lastX, lastY;
	int xpos, ypos;
	SDL_GetMouseState(&xpos, &ypos);
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = static_cast<float>(xpos - lastX);
	float yoffset = static_cast<float>(lastY - ypos);
	lastX = xpos;
	lastY = ypos;

	setCamRotation(xoffset, yoffset);
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Cube3D::onMouseScroll(float yoffset)
{
	auto glProperty = m_pCube3D->GetPropertyMethodObj<GLProperty>();
	float Zoom = glProperty->GetFOV();
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
	{
		Zoom = 1.0f;
	}
	else if (Zoom > 45.0f)
	{
		Zoom = 45.0f;
	}
	glProperty->SetFOV(Zoom);
}

void Cube3D::setVisible(bool visible)
{
	m_pCube3D->SetPropertyValue(VISIBLE, visible);
}

Node3DPtr Cube3D::getCube3D()
{
	return m_pCube3D;
}

void Cube3D::init(VoidType&& dummy)
{
	LOG_DEBUG("");
	AcquireResource();
	Scene::GetInstance()->AddToSceneGraph(m_pCube3D);
	auto pMaterial = Library::GetInstance()->get<Material>(CUBE_3D_URL);
	ImGuiShader::GetInstance()->importProperty(pMaterial);
	ImGuiShader::GetInstance()->importProperty(m_pCube3D);
}

void Cube3D::AcquireResource()
{
	LOG_DEBUG("");
	m_pCube3D = Scene::GetInstance()->LookupUIObject<Node3D>(CUBE_3D_URL);
}

void Cube3D::setCamRotation(float xoffset, float yoffset)
{
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	auto glProperty = m_pCube3D->GetPropertyMethodObj<GLProperty>();
	auto yaw = glProperty->GetCamYaw();
	auto pitch = glProperty->GetCamPitch();

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	glProperty->SetCamFront(glm::normalize(direction));
	glProperty->SetCamPitch(pitch);
	glProperty->SetCamYaw(yaw);
}

void Cube3D::lookAtMe()
{
	auto glProperty = m_pCube3D->GetPropertyMethodObj<GLProperty>();
	auto cameraPos		= glProperty->GetCamPos();
	auto cameraTarget	= glm::vec3(0.0F);
	auto cameraFront	= glm::normalize(cameraTarget - cameraPos);
	glProperty->SetCamFront(cameraFront);
}