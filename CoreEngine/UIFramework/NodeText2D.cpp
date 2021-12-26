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
#include "Renderer3D.h"
NodeText2D::NodeText2D(std::string name):UIObject(name)
{
	AddPropertyMethodObj(TextProperty::create(dynamic_cast<PropertyTable*>(this)));

	std::function<void(std::string&&)> onTextUpdate = [&](std::string&&) {m_bFontChanged = m_bUIChanged = true; };
	std::function<void(int32_t&&)> onFontSizeUpdate = [&](int32_t&&) {m_bFontChanged = m_bUIChanged = true; };
#ifdef OPENGL_RENDERING
	std::function<void(glm::vec4&&)> onColorUpdate = [&](glm::vec4&& value) {m_bUIChanged = true; };
	std::function<void(glm::vec3&&)> onFontColorUpdate = [&](glm::vec3&& value) {m_bUIChanged = true; };
#else
	std::function<void(SDL_Color&&)> onColorUpdate = [&](SDL_Color&&) {m_bFontChanged = m_bUIChanged = true; };
	std::function<void(SDL_Color&&)> onFontColorUpdate = [&](SDL_Color&&) {m_bFontChanged = m_bUIChanged = true; };
#endif
	
	//<on text propert changed
	BindPropertySignal(TEXT, onTextUpdate);
	BindPropertySignal(FONT_NAME, onTextUpdate);
	BindPropertySignal(FONT_SIZE, onFontSizeUpdate);
	BindPropertySignal(FONT_COLOR, onFontColorUpdate);

	//<on foreground color changed
	BindPropertySignal(FORE_GROUND_COLOR, onColorUpdate);

	//<on draw
	bind(ON_DRAW_SIGNAL, this, &NodeText2D::onDraw);

	//<on scale changed
	std::function<void(float&&)> onScaleUpdate = [&](float&& value) {m_bUIChanged = true;	};
	BindPropertySignal(SCALE_X, onScaleUpdate);
	BindPropertySignal(SCALE_Y, onScaleUpdate);

	//<on alpha changed
#ifdef OPENGL_RENDERING
	std::function<void(float&&)> onOpacityUpdate = [&](float&& value) {m_bUIChanged = true; };
#else
	std::function<void(uint8_t&&)> onOpacityUpdate = [&](uint8_t&& value) {m_bUIChanged = true;	};
#endif
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

	//get center point
	SDL_Point centerPoint = originMethod->GetCenterPoint();

	SDL_Rect display_rect = layoutMethod->GetLayoutInformation();

	//get center point
	auto centerPoint = originMethod->GetCenterPoint();

#endif


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
			TextureManipulator(m_pFont).ColorKey(color);
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
	auto color = TextMethod->GetColor();

	//<set world matrix
	Renderer3D::GetInstance()->setModalMatrix(m_worldTransform);

	//Render image
	Renderer3D::GetInstance()->DrawText2D(m_pGLFont->getCharacterList(), originMethod->GetOpacity(), color);
#endif
}

void NodeText2D::onClean(VoidType&&)
{}