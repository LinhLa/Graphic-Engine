#pragma once
#include <Node3D.h>

#define CUBE_3D_URL 	"3D Cube"

class Cube3D final: public creator<Cube3D>, public std::enable_shared_from_this<Cube3D>
{
private:
	Node3DPtr		m_pCube3D = nullptr;
	void AcquireResource();
	void onKeyPress();
	void onMouseScroll(float yoffset);
	void onMouseMove();
	void setCamRotation(float xoffset, float yoffset);
	void lookAt();
protected:
	Cube3D();
public:
	~Cube3D();
	friend class creator<Cube3D>;

	void setVisible(bool visible);

	Node3DPtr	getCube3D();

	void init(VoidType&&);
	void onKeyInputEvent(SDL_Event&& arg);
};
typedef std::shared_ptr<Cube3D> Cube3DPtr;