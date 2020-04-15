#pragma once
#include <Node2DImage.h>
#include <NodeText2D.h>

enum ButtonSprite
{
	BUTTON_SPRITE_NORMAL = 0,
    BUTTON_SPRITE_MOUSE_OUT,
    BUTTON_SPRITE_MOUSE_OVER_MOTION,
    BUTTON_SPRITE_MOUSE_DOWN,
    BUTTON_SPRITE_MOUSE_UP,
    BUTTON_SPRITE_TOTAL,
};

class NodeButton2D : public GameObject
{
private:
	Node2DImage*	m_pBackground_texture = nullptr;
	NodeText2D*		m_pText_texture = nullptr;
	ButtonSprite 	m_CurrentSprite = BUTTON_SPRITE_NORMAL;
public:
	enum {TYPE = NODE_2D_BUTTON_TYPE,};

	NodeButton2D();
	~NodeButton2D();


	void onKeyInputEvent(KeyInputArgumentType&) override;
	void onInit(RenderInitArgumentType&) override;
	void onDraw(RenderDrawArgumentType&) override;
	void onClean(RenderCleanArgumentType&) override;
	void onLoop(RenderLoopArgumentType&) override;

};

typedef std::shared_ptr<NodeButton2D> NodeButton2DPtr;