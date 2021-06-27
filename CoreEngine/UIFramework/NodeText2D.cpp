#include "stdafx.h"
#include <NodeText2D.h>
#include <Configuration.h>

#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "TextProperty.h"

#include "UIHelper.h"
#include "RenderManipulator.h"
#include "TextureManipulator.h"

#include "PropertyDefine.h"
#include "SignalDefine.h"

NodeText2D::NodeText2D(std::string name):UIObject(name)
{
	AddPropertyMethodObj(TextProperty::create(dynamic_cast<PropertyTable*>(this)));

	std::function<void(std::string&&)> strCallback = [&](std::string&&) {m_bFontChanged = m_bUIChanged = true; };
	std::function<void(int32_t&&)> i32Callback = [&](int32_t&&) {m_bFontChanged = m_bUIChanged = true; };
	std::function<void(SDL_Color&&)> sdlColorCallback = [&](SDL_Color&&) {m_bFontChanged = m_bUIChanged = true; };

	//<on text propert changed
	BindPropertySignal(TEXT, strCallback);
	BindPropertySignal(FONT_NAME, strCallback);
	BindPropertySignal(FONT_SIZE, i32Callback);
	BindPropertySignal(FONT_COLOR, sdlColorCallback);

	//<on foreground color changed
	BindPropertySignal(FORE_GROUND_COLOR, sdlColorCallback);

	//<on draw
	bind(ON_DRAW_SIGNAL, this, &NodeText2D::onDraw);

	//<on scale changed
	std::function<void(float&&)> floatCallback = [&](float&& value) {m_bUIChanged = true;	};
	BindPropertySignal(SCALE_X, floatCallback);
	BindPropertySignal(SCALE_Y, floatCallback);

	//<on alpha changed
	std::function<void(uint8_t&&)> u8Callback = [&](uint8_t&& value) {m_bUIChanged = true;	};
	BindPropertySignal(OPACITY, u8Callback);
}

NodeText2D::~NodeText2D(){}

void NodeText2D::UpdateFont()
{
	if (!m_bFontChanged)
	{
		return;
	}

	TextPropertyPtr TextMethod = GetPropertyMethodObj<TextProperty>();
	if (TextMethod)
	{
#ifdef OPENGL_RENDERING

#else
		//create new font
		m_pFont = Font::create();

		//open font
		m_pFont->OpenFont(
			TextMethod->GetFontName().c_str(),
			TextMethod->GetFontSize());

		//load font texture
		m_pFont->loadFont(
			TextMethod->GetText().c_str(),
			TextMethod->GetColor(),
			UIHelper::GetRenderer());
#endif
	}

	//reset flag
	m_bFontChanged = false;
}

uint8_t NodeText2D::getType()
{
	return NODE_2D_TEXT_TYPE;
}

void NodeText2D::onInit(VoidType&&)
{}

void NodeText2D::onDraw(VoidType&&)
{
	auto OriginMethod = GetPropertyMethodObj<OriginProperty>();
	auto layoutMethod = GetPropertyMethodObj<LayoutProperty>();
#ifndef OPENGL_RENDERING
	//Open new font and create texture
	UpdateFont();
	SDL_Rect sdlResult{ 0, 0, m_pFont->GetWidth(), m_pFont->GetHeight() };
	SDL_Rect display_rect = layoutMethod->GetLayoutInformation();

	//get center point
	SDL_Point centerPoint = OriginMethod->GetCenterPoint();

	if (true == m_bUIChanged)
	{
		//calculate scale
		display_rect = UIHelper::GetScaleRect(
			display_rect.x,
			display_rect.y,
			m_pFont->GetWidth(),
			m_pFont->GetHeight(),
			layoutMethod->GetLayoutScaleX(),
			layoutMethod->GetLayoutScaleY());

		//check fore ground color
		if (IsPropertyExist(FORE_GROUND_COLOR))
		{
			SDL_Color color = OriginMethod->GetForeGroundColor();
			if (IS_VALID_COLOR(color))
			{
				TextureManipulator(m_pFont).ColorKey(color);
			}
		}

		//set alpha
		m_pFont->setAlpha(OriginMethod->GetOpacity());

		//reset flag
		m_bUIChanged = false;
	}

	//<load texture to render
	RenderExContextPtr context = RenderExContext::create(UIHelper::GetRenderer(), sdlResult, display_rect, OriginMethod->GetAngle(), centerPoint, OriginMethod->GetFlip());
	context->excute(m_pFont);
#else

#endif
}

void NodeText2D::onClean(VoidType&&)
{}