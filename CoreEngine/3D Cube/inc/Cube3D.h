#pragma once
#include <Node3D.h>
#include <NodeCamera.h>
#include <NodeLight.h>

class Cube3D final: public creator<Cube3D>, public std::enable_shared_from_this<Cube3D>
{
private:
	bool KEYS[SDL_NUM_SCANCODES];
	bool OLD_KEYS[SDL_NUM_SCANCODES];

	NodeMeshPtr		m_pShere = nullptr;
	Node3DPtr		m_pCube3D = nullptr;
	NodeCameraPtr	m_pFirsPersonCam = nullptr;
	NodeCameraPtr	m_pThirdPersonCam = nullptr;
	NodePointLightPtr m_pLamp = nullptr;

	void AcquireResource();
	bool isKeyHold(std::initializer_list<SDL_Scancode>);
	void onKeyPress();
	void onMouseScroll(float yoffset);
	void onMouseMove();
	void setCamRotation(float xoffset, float yoffset);
	void lookAtMe();
protected:
	Cube3D();
public:
	~Cube3D();
	friend class creator<Cube3D>;

	void setVisible(bool visible);

	Node3DPtr	getCube3D();

	void init(VoidType&&);
	void onKeyInputEvent(SDL_Event&& arg);
	void onRenderEvent(SDL_Event&& arg);
};
typedef std::shared_ptr<Cube3D> Cube3DPtr;