#pragma once
#include <list>
#include <MyCustomAlloc.h>
#include <GameObject.h>
#include <Texture.h>
#include <Animation.h>

class IEntity: public GameObject
{
private:
	Animation   m_AnimationControl;
	Texture		m_EntityTexture;
	int32_t     AnimationState = 0;

public:
	IEntity();
	virtual ~IEntity();

	void loadTexture(const char*, uint32_t MaxFrames, SDL_Renderer*);

	void onKeyInputEvent(KeyInputArgumentType&) override;
	void onInit(RenderInitArgumentType&) override;
	void onDraw(RenderDrawArgumentType&) override;
	void onClean(RenderCleanArgumentType&) override;
	void onLoop(RenderLoopArgumentType&) override;
};