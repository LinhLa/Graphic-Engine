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

#include "GLRenderContext.h"

NodeText2D::NodeText2D(std::string name):UIObject(name)
{
	AddPropertyMethodObj(TextProperty::create(dynamic_cast<PropertyTable*>(this)));

	std::function<void(std::string&&)> onTextUpdate = [&](std::string&&) {m_bFontChanged = m_bUIChanged = true; };
	std::function<void(int32_t&&)> onFontSizeUpdate = [&](int32_t&&) {m_bFontChanged = m_bUIChanged = true; };
	std::function<void(SDL_Color&&)> onColorUpdate = [&](SDL_Color&&) {m_bFontChanged = m_bUIChanged = true; };

	//<on text propert changed
	BindPropertySignal(TEXT, onTextUpdate);
	BindPropertySignal(FONT_NAME, onTextUpdate);
	BindPropertySignal(FONT_SIZE, onFontSizeUpdate);
	BindPropertySignal(FONT_COLOR, onColorUpdate);

	//<on foreground color changed
	BindPropertySignal(FORE_GROUND_COLOR, onColorUpdate);

	//<on draw
	bind(ON_DRAW_SIGNAL, this, &NodeText2D::onDraw);

	//<on scale changed
	std::function<void(float&&)> onScaleUpdate = [&](float&& value) {m_bUIChanged = true;	};
	BindPropertySignal(SCALE_X, onScaleUpdate);
	BindPropertySignal(SCALE_Y, onScaleUpdate);

	//<on alpha changed
	std::function<void(uint8_t&&)> onOpacityUpdate = [&](uint8_t&& value) {m_bUIChanged = true;	};
	BindPropertySignal(OPACITY, onOpacityUpdate);

	//<create texture to render GLTexture(const std::string& name, GLenum target, int w, int h, GLint format);
	m_pTexture = GLTexture::create(m_name, GL_SAMPLER_2D, Configuration::GetInstance()->width, Configuration::GetInstance()->height, GL_RGB);
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
		//create new font
		m_pGLFont = GLFont::create();

		//open font
		m_pGLFont->OpenFont(
			TextMethod->GetFontName().c_str(),
			TextMethod->GetFontSize());

		//load font texture
		m_pGLFont->loadFont(TextMethod->GetText().c_str());
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
	auto originMethod = GetPropertyMethodObj<OriginProperty>();
	auto layoutMethod = GetPropertyMethodObj<LayoutProperty>();
	auto TextMethod = GetPropertyMethodObj<TextProperty>();
	//Open new font and create texture
	UpdateFont();
#ifndef OPENGL_RENDERING
	SDL_Rect sdlResult{ 0, 0, m_pFont->GetWidth(), m_pFont->GetHeight() };
#endif
	SDL_Rect display_rect = layoutMethod->GetLayoutInformation();

	//get center point
	SDL_Point centerPoint = originMethod->GetCenterPoint();

	if (true == m_bUIChanged)
	{
#ifndef OPENGL_RENDERING
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
		m_pFont->setAlpha(originMethod->GetOpacity());
#endif
		//reset flag
		m_bUIChanged = false;
	}
#ifndef OPENGL_RENDERING
	//<load texture to render
	RenderExContextPtr context = RenderExContext::create(UIHelper::GetRenderer(), sdlResult, display_rect, OriginMethod->GetAngle(), centerPoint, OriginMethod->GetFlip());
	context->excute(m_pFont);
#else
	//get font color
	SDL_Color color = TextMethod->GetColor();

	//get layout scale
	auto scale = layoutMethod->GetLayoutScale();

	//<load texture
	m_pGLFont->renderTexture(
		m_pTexture,
		glm::vec2(static_cast<float>(display_rect.x), static_cast<float>(display_rect.y)),
		glm::vec2(scale.x, scale.y),
		static_cast<float>(originMethod->GetAngle()),
		glm::vec2(static_cast<float>(centerPoint.x), static_cast<float>(centerPoint.y)),
		glm::vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f)
	);

	//<Render
	auto context = GLRender2DContext::create(
		m_pTexture,
		glm::vec2(0.0f),
		glm::vec2(1.0f),
		glm::vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f),
		0.0f,
		1.0f,
		glm::vec2(centerPoint.x, centerPoint.y),
		SDL_FLIP_NONE);

	//context->excute();
#endif
}

void NodeText2D::onClean(VoidType&&)
{}