#include "stdafx.h"
#include <NodeViewPort2D.h>
#include "LayoutProperty.h"
#include "OriginProperty.h"
#include "UIHelper.h"
#include "SignalDefine.h"

NodeViewPort2D::NodeViewPort2D(std::string name):UIObject(name)
{
	bind(ON_DRAW_SIGNAL, this, &NodeViewPort2D::onDraw);
	bind(ON_INIT_SIGNAL, this, &NodeViewPort2D::onInit);
}

NodeViewPort2D::~NodeViewPort2D() {}

uint8_t NodeViewPort2D::getType()
{
	return NODE_2D_VIEWPORT_TYPE;
}

void NodeViewPort2D::onInit(VoidType&&)
{
	auto layoutMethod = GetPropertyMethodObj<LayoutProperty>();
	//<load texture to render
	SDL_Rect display_rect = layoutMethod->GetLayoutInformation();
	SDL_RenderSetViewport(UIHelper::GetRenderer(), &display_rect);
}

void NodeViewPort2D::onDraw(VoidType&&)
{
	auto OriginMethod = GetPropertyMethodObj<OriginProperty>();
	auto LayoutMethod = GetPropertyMethodObj<LayoutProperty>();
	if (!OriginMethod->isVisible())
	{
		return;
	}

	SDL_Rect display_rect = LayoutMethod->GetLayoutInformation();

	//Render texture to screen
	RenderContextPtr context = RenderContext::create(UIHelper::GetRenderer(), display_rect, display_rect);
	context->excute(m_pTexture);
}

void NodeViewPort2D::onClean(VoidType&&)
{}
