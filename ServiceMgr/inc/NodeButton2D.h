#pragma once
#include <Node2DImage.h>
#include <NodeText2D.h>

class NodeButton2D : public GameObject
{
private:
	Node2DImage*	m_pBackground_texture = nullptr;
	NodeText2D*		m_pText_texture = nullptr;
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