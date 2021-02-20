#include "stdafx.h"

#include <base_define.h>
#include <Node2DImage.h>
#include "Library.h"

#include "OriginProperty.h"
#include "LayoutProperty.h"

#include "UIHelper.h"
#include "RenderManipulator.h"
#include "TextureManipulator.h"

#include "PropertyDefine.h"
#include "SignalDefine.h"

Node2DImage::Node2DImage(std::string name):UIObject(name)
{
	//<on draw
	bind(ON_DRAW_SIGNAL, this, &Node2DImage::onDraw);

	//<set flag ui update on foreground color changed
	std::function<void(SDL_Color&&)> sdlColorCallback = [&](SDL_Color&& value) {m_bUIChanged = true; };
	BindPropertySignal(FORE_GROUND_COLOR, sdlColorCallback);

	//<set flag ui update on scale
	std::function<void(float&&)> floatCallback = [&](float&& value) {m_bUIChanged = true;	};
	BindPropertySignal(SCALE_X, floatCallback);
	BindPropertySignal(SCALE_Y, floatCallback);

	//set flag ui update <on alpha changed
	std::function<void(uint8_t&&)> u8Callback = [&](uint8_t&& value) {m_bUIChanged = true;	};
	BindPropertySignal(OPACITY, u8Callback);
}

Node2DImage::~Node2DImage() {}

uint8_t Node2DImage::getType()
{
	return NODE_2D_IMAGE_TYPE;
}

void Node2DImage::onInit(VoidType&&)
{}

void Node2DImage::onDraw(VoidType&&)
{
	auto originMethod = GetPropertyMethodObj<OriginProperty>();
	auto layoutMethod = GetPropertyMethodObj<LayoutProperty>();

	//get layout display
	SDL_Rect sdlResult{0, 0, m_pTexture->GetWidth(), m_pTexture->GetHeight()};
	SDL_Rect display_rect = layoutMethod->GetLayoutInformation();

	//get center point
	SDL_Point centerPoint  = originMethod->GetCenterPoint();

	if (true == m_bUIChanged)
	{
		//calculate scale
		display_rect = UIHelper::GetScaleRect(
			display_rect.x,
			display_rect.y,
			m_pTextureToRender->GetWidth(),
			m_pTextureToRender->GetHeight(),
			layoutMethod->GetLayoutScaleX(),
			layoutMethod->GetLayoutScaleY());

		//check fore ground color
		if (IsPropertyExist(FORE_GROUND_COLOR))
		{
			SDL_Color color = originMethod->GetForeGroundColor();
			if (IS_VALID_COLOR(color))
			{
				//clone texture
				m_pTextureToRender = m_pTexture->Clone(UIHelper::GetRenderer());
				TextureManipulator(m_pTextureToRender).ColorKey(color);
			}
		}

		//set opacity
		m_pTextureToRender->setAlpha(originMethod->GetOpacity());

		//reset flag
		m_bUIChanged = false;
	}

	//<load texture to render
	RenderExContextPtr context = RenderExContext::create(UIHelper::GetRenderer(), sdlResult, display_rect, originMethod->GetAngle(), centerPoint, originMethod->GetFlip());
	context->excute(m_pTextureToRender);
}

void Node2DImage::onClean(VoidType&&)
{}

int Node2DImage::GetActualWidth() const
{
	return m_pTexture->GetWidth();
}

int Node2DImage::GetActualHeight() const
{
	return m_pTexture->GetHeight();
}

void Node2DImage::SetTexture(const std::string& texture_name)
{
	m_pTexture = Library::GetInstance()->getTexture(texture_name);

	//clone texture
	m_pTextureToRender = m_pTexture->Clone(UIHelper::GetRenderer());

	//set flag ui update
	m_bUIChanged = true;
}