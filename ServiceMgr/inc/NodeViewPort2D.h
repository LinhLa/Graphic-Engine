#pragma once
#include <GameObject.h>
#include <Texture.h>

class NodeViewPort2D : public GameObject
{
private:
	Texture m_texture;
public:
	enum { TYPE = NODE_2D_VIEWPORT_TYPE, };

	NodeViewPort2D();
	~NodeViewPort2D();


	void onKeyInputEvent(KeyInputArgumentType&) override;
	void onInit(RenderInitArgumentType&) override;
	void onDraw(RenderDrawArgumentType&) override;
	void onClean(RenderCleanArgumentType&) override;
	void onLoop(RenderLoopArgumentType&) override;

};

typedef std::shared_ptr<NodeViewPort2D> NodeViewPort2DPtr;