#pragma once
#include <GameObject.h>
#include <Texture.h>
class Node2DImage: public GameObject
{
private:
	Texture m_texture_background;
public:
	enum { TYPE = NODE_2D_IMAGE_TYPE, };

	Node2DImage();
	~Node2DImage();


	void onKeyInputEvent(KeyInputArgumentType&) override;
	void onInit(RenderInitArgumentType&) override;
	void onDraw(RenderDrawArgumentType&) override;
	void onClean(RenderCleanArgumentType&) override;
	void onLoop(RenderLoopArgumentType&) override;

};

typedef std::shared_ptr<Node2DImage> Node2DImagePtr;