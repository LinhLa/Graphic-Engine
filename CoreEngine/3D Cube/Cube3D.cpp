#include "stdafx.h"
#include <chrono>
#include "Cube3D.h"

#include "Scene.h"
#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "cameraProperty.h"
#include "CameraProperty.h"

#include "PropertyDefine.h"

#include "deltatime.h"
#include "UIHelper.h"
#include "SignalDefine.h"

#include "AnimationTimeLine.h"
#include "time_suffix.h"

#include "Library.h"
#include "Material.h"

#define CLOCKS_PER_MILISECOND (CLOCKS_PER_SEC / 1000)

#define CUBE_3D_URL 		"3D Cube"
#define FIRST_PERSON_CAM	"First Person"
#define THIRD_PERSON_CAM	"Third Person"
#define LAMP_POINT_LIGHT	"Lamb"
#define SPHERE				"sphere1"
#define DIRECTION			"direction"
#define DISTANCE_UP			"distance up"
#define DISTANCE_AWAY		"distance away"

using namespace UIHelper;

Cube3D::Cube3D()
{
	memset(KEYS, 0, sizeof(KEYS));
	memset(OLD_KEYS, 0, sizeof(OLD_KEYS));
}
Cube3D::~Cube3D() {}

void Cube3D::onKeyInputEvent(SDL_Event&& arg)
{
	//LOG_DEBUG("");
	switch (arg.type)
	{
	case SDL_MOUSEWHEEL:
	{
		if (0 != arg.wheel.y) // scroll up/down
		{
			onMouseScroll(static_cast<float>(arg.wheel.y));
		}
		else
		{
		}
		break;
	}
	case SDL_MOUSEMOTION:
	{
		//onMouseMove();
		break;
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		break;
	}
	case SDL_KEYDOWN:
	{
		OLD_KEYS[arg.key.keysym.scancode] = KEYS[arg.key.keysym.scancode];
		KEYS[arg.key.keysym.scancode] = true;
		onKeyPress();
		break;
	}
	case SDL_KEYUP:
	{
		OLD_KEYS[arg.key.keysym.scancode] = KEYS[arg.key.keysym.scancode];
		KEYS[arg.key.keysym.scancode] = false;
		break;
	}
	default:
		break;
	}
}

bool Cube3D::isKeyHold(std::initializer_list<SDL_Scancode> list)
{
	bool bResult = true;
	for (auto& keycode : list)
	{
		bResult = bResult && (OLD_KEYS[keycode] && KEYS[keycode]);
	}
	return bResult;
}

void Cube3D::onKeyPress()
{
	//<ignore exceptional case
	if (isKeyHold({ SDL_SCANCODE_W, SDL_SCANCODE_S }) ||
		isKeyHold({ SDL_SCANCODE_A , SDL_SCANCODE_D }))
	{
		//do nothing
		return;
	}

	const uint8_t* currentKeyStates = SDL_GetKeyboardState(NULL);
	float CamSpeed = 0.001f * static_cast<float>(DeltaTime::GetInstance()->getDelta());
	float MovementSpeed = 0.001f * static_cast<float>(DeltaTime::GetInstance()->getDelta());
	//auto cameraProperty = m_pFirsPersonCam->GetPropertyMethodObj<CameraProperty>();
	auto cameraProperty = m_pThirdPersonCam->GetPropertyMethodObj<CameraProperty>();
	auto cameraPos = cameraProperty->GetCamPos();
	auto cameraTarget = cameraProperty->GetCamTarget();
	auto cameraUp = cameraProperty->GetCamUp();
	auto cameraFront = cameraProperty->GetCamFront();

	auto targetPos = m_pShere->GetPropertyValue<glm::vec3>(TRANS_VECTOR);
	auto targetDir = m_pShere->GetPropertyValue<glm::vec3>(DIRECTION);
	bool bLookAtMe = false;


	//<Handle NPC movement
	if (isKeyHold({ SDL_SCANCODE_A, SDL_SCANCODE_W }) ||
		isKeyHold({ SDL_SCANCODE_A, SDL_SCANCODE_S }) ||
		isKeyHold({ SDL_SCANCODE_W, SDL_SCANCODE_D }) ||
		isKeyHold({ SDL_SCANCODE_D, SDL_SCANCODE_S }))
	{
		targetPos += targetDir * MovementSpeed;
	}
	else if (currentKeyStates[SDL_SCANCODE_A] && currentKeyStates[SDL_SCANCODE_W])
	{
		targetDir = glm::normalize(cameraFront -glm::cross(cameraFront, cameraUp));
		targetPos += targetDir * MovementSpeed;
	}
	else if (currentKeyStates[SDL_SCANCODE_W] && currentKeyStates[SDL_SCANCODE_D])
	{
		targetDir = glm::normalize(cameraFront + glm::cross(cameraFront, cameraUp));
		targetPos += targetDir * MovementSpeed;
	}
	else if (currentKeyStates[SDL_SCANCODE_D] && currentKeyStates[SDL_SCANCODE_S])
	{
		targetDir = glm::normalize(-cameraFront + glm::cross(cameraFront, cameraUp));
		targetPos += targetDir * MovementSpeed;
	}
	else if (currentKeyStates[SDL_SCANCODE_A] && currentKeyStates[SDL_SCANCODE_S])
	{
		targetDir = glm::normalize(-cameraFront -glm::cross(cameraFront, cameraUp));
		targetPos += targetDir * MovementSpeed;
	}
	else if (currentKeyStates[SDL_SCANCODE_A])
	{
		targetDir = -glm::normalize(glm::cross(cameraFront, cameraUp));
		targetPos += targetDir * MovementSpeed;
	}
	else if (currentKeyStates[SDL_SCANCODE_W])
	{
		targetDir = cameraFront;
		targetPos += targetDir * MovementSpeed;
	}
	else if (currentKeyStates[SDL_SCANCODE_S])
	{
		targetDir = -cameraFront;
		targetPos += targetDir * MovementSpeed;
	}
	else if (currentKeyStates[SDL_SCANCODE_D])
	{
		targetDir = glm::normalize(glm::cross(cameraFront, cameraUp));
		targetPos += targetDir * MovementSpeed;
	}
	else {}

	/*float radius = glm::distance(cameraPos, glm::vec3(0.0f));
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
	}*/

	//cameraProperty->SetCamPos(cameraPos);
	m_pShere->SetPropertyValue(TRANS_VECTOR, targetPos);
	m_pShere->SetPropertyValue(DIRECTION, targetDir);
	//if (bLookAtMe)
	//{
	//	lookAtMe();
	//}

	//<Handle camera movement
	auto targetDisUp = m_pShere->GetPropertyValue<float>(DISTANCE_UP);
	auto targetDisAway = m_pShere->GetPropertyValue<float>(DISTANCE_AWAY);

	if (!KEYS[SDL_SCANCODE_S])
	{
		auto newCamTarget = targetPos + glm::vec3(0.0f, targetDisUp, 0.0f);
		auto newCamPos = newCamTarget - (targetDir * targetDisAway);
		auto newCamFront = glm::normalize(newCamTarget - newCamPos);

		cameraPos += (newCamPos - cameraPos) * CamSpeed;
		cameraTarget += (newCamTarget - cameraTarget) * CamSpeed;
		cameraFront += (newCamFront - cameraFront) * CamSpeed;

		cameraProperty->SetCamTarget(cameraTarget);
		cameraProperty->SetCamPos(cameraPos);
		cameraProperty->SetCamFront(glm::normalize(cameraFront));

	}
	else
	{
		//<Camera keep direction and move backward when NPC go directly to camera
		auto newCamTarget = targetPos + glm::vec3(0.0f, targetDisUp, 0.0f);
		auto newCamPos = newCamTarget + (-cameraFront * targetDisAway);
		//auto newCamFront = glm::normalize(newCamTarget - newCamPos);

		cameraPos += (newCamPos - cameraPos) * CamSpeed;
		cameraTarget += (newCamTarget - cameraTarget) * CamSpeed;
		//cameraFront += (newCamFront - cameraFront) * CamSpeed;

		cameraProperty->SetCamTarget(cameraTarget);
		cameraProperty->SetCamPos(cameraPos);
		//cameraProperty->SetCamFront(glm::normalize(cameraFront));
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
	//float Zoom = m_pFirsPersonCam->GetPropertyValue<float>(FOV);
	float Zoom = m_pThirdPersonCam->GetPropertyValue<float>(FOV);
	Zoom -= (float)yoffset;
	if (Zoom < 1.0f)
	{
		Zoom = 1.0f;
	}
	else if (Zoom > 45.0f)
	{
		Zoom = 45.0f;
	}
	//m_pFirsPersonCam->SetPropertyValue<float>(FOV, std::move(Zoom));
	m_pThirdPersonCam->SetPropertyValue<float>(FOV, std::move(Zoom));
}

void Cube3D::onRenderEvent(SDL_Event&& arg)
{
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

	//<Setup scene graph
	Scene::GetInstance()->AddToSceneGraph(m_pCube3D);

	auto cameraProperty = m_pThirdPersonCam->GetPropertyMethodObj<CameraProperty>();
	auto cameraPos = cameraProperty->GetCamPos();
	auto cameraTarget = cameraProperty->GetCamTarget();
	auto cameraUp = cameraProperty->GetCamUp();
	auto cameraFront = cameraProperty->GetCamFront();

	auto targetPos = m_pShere->GetPropertyValue<glm::vec3>(TRANS_VECTOR);
	auto targetDir = m_pShere->GetPropertyValue<glm::vec3>(DIRECTION);
	auto targetDisUp = m_pShere->GetPropertyValue<float>(DISTANCE_UP);
	auto targetDisAway = m_pShere->GetPropertyValue<float>(DISTANCE_AWAY);

	auto newCamPos = (targetPos + glm::vec3(0.0f, targetDisUp, 0.0f)) - (targetDir * targetDisAway);
	auto newCamTarget = targetPos + glm::vec3(0.0f, targetDisUp, 0.0f);
	auto newCamFront = glm::normalize(newCamTarget - newCamPos);

	cameraPos += (newCamPos - cameraPos);
	cameraTarget += (newCamTarget - cameraTarget);
	cameraFront += (newCamFront - cameraFront);

	cameraProperty->SetCamTarget(cameraTarget);
	cameraProperty->SetCamPos(cameraPos);
	cameraProperty->SetCamFront(glm::normalize(cameraFront));
}

void Cube3D::AcquireResource()
{
	LOG_DEBUG("");
	m_pCube3D = Scene::GetInstance()->LookupUIObject<Node3D>(CUBE_3D_URL);
	m_pFirsPersonCam = m_pCube3D->getChild<NodeCamera>(FIRST_PERSON_CAM);
	m_pThirdPersonCam = m_pCube3D->getChild<NodeCamera>(THIRD_PERSON_CAM);
	m_pLamp = m_pCube3D->getChild<NodePointLight>(LAMP_POINT_LIGHT);
	m_pShere = m_pCube3D->getChild<NodeMesh>(SPHERE);

	m_pCube3D->SetNodeLight<NodePointLight>({ m_pLamp });
	//m_pCube3D->SetNodeCamera(m_pFirsPersonCam);
	m_pCube3D->SetNodeCamera(m_pThirdPersonCam);

	m_pShere->AddProperty(DIRECTION, Property<glm::vec3>::create(DIRECTION, glm::vec3(0.0f, 0.0f, -1.0f), GL_FLOAT_VEC3));
	m_pShere->AddProperty(DISTANCE_UP, Property<float>::create(DISTANCE_UP, 0.2f, GL_FLOAT));
	m_pShere->AddProperty(DISTANCE_AWAY, Property<float>::create(DISTANCE_AWAY, 5.0f, GL_FLOAT));
}

void Cube3D::setCamRotation(float xoffset, float yoffset)
{
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	auto cameraProperty = m_pFirsPersonCam->GetPropertyMethodObj<CameraProperty>();
	auto yaw = cameraProperty->GetCamYaw();
	auto pitch = cameraProperty->GetCamPitch();

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

	cameraProperty->SetCamFront(glm::normalize(direction));
	cameraProperty->SetCamPitch(pitch);
	cameraProperty->SetCamYaw(yaw);
}

void Cube3D::lookAtMe()
{
	auto cameraProperty = m_pFirsPersonCam->GetPropertyMethodObj<CameraProperty>();
	auto cameraPos = cameraProperty->GetCamPos();
	auto cameraTarget = glm::vec3(0.0F);
	auto cameraFront = glm::normalize(cameraTarget - cameraPos);
	cameraProperty->SetCamFront(cameraFront);
}