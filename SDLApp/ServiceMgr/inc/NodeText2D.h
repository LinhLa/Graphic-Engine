#pragma once
#include <GameObject.h>
#include <Font.h>
class NodeText2D : public GameObject, public TextProperty
{
private:
	Font		m_FontTexture;
	TTF_Font*	m_font = nullptr;

public:
	enum { TYPE = NODE_2D_TEXT_TYPE, };

	NodeText2D();
	~NodeText2D();

	void onKeyInputEvent(KeyInputArgumentType&) override;
	void onInit(RenderInitArgumentType&) override;
	void onDraw(RenderDrawArgumentType&) override;
	void onClean(RenderCleanArgumentType&) override;
	void onLoop(RenderLoopArgumentType&) override;

};

typedef std::shared_ptr<NodeText2D> NodeText2DPtr;