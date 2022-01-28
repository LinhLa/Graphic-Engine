#include "stdafx.h"
#include "utils.h"
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

#include "GLRenderContext.h"
#include "GLRenderManipulator.h"
#include "Renderer3D.h"

Node2DImage::Node2DImage(std::string name) :UIObject(name)
{
	//<on draw
	bind(ON_DRAW_SIGNAL, this, &Node2DImage::onDraw);

	//<set flag ui update on foreground color changed
#ifdef OPENGL_RENDERING
	std::function<void(glm::vec4&&)> onColorForeground = [&](glm::vec4&& value) {m_bUIChanged = true; };
#else
	std::function<void(SDL_Color&&)> onColorForeground = [&](SDL_Color&& value) {m_bUIChanged = true; };
#endif

	BindPropertySignal(FORE_GROUND_COLOR, onColorForeground);

	//<set flag ui update on scale
	std::function<void(float&&)> onScale = [&](float&& value) {m_bUIChanged = true;	};
	BindPropertySignal(SCALE_X, onScale);
	BindPropertySignal(SCALE_Y, onScale);

	//set flag ui update on alpha changed
#ifdef OPENGL_RENDERING
	std::function<void(float&&)> onOpacityUpdate = [&](float&& value) {m_bUIChanged = true; };
#else
	std::function<void(uint8_t&&)> onOpacityUpdate = [&](uint8_t&& value) {m_bUIChanged = true;	};
#endif
	BindPropertySignal(OPACITY, onOpacityUpdate);
}

Node2DImage::~Node2DImage() {}

uint8_t Node2DImage::getType()
{
	return NODE_2D_IMAGE_TYPE;
}

UIObjectPtr Node2DImage::clone()
{
	auto pObject = Node2DImage::create(m_name);
	this->setProperty(pObject);
	pObject->SetTexture(m_pTexture->getName());
	return pObject;
}

void Node2DImage::onInit(VoidType&&)
{}

void Node2DImage::onDraw(VoidType&&)
{
	auto originMethod = GetPropertyMethodObj<OriginProperty>();
	auto layoutMethod = GetPropertyMethodObj<LayoutProperty>();

	//get layout display
#ifndef OPENGL_RENDERING
	SDL_Rect sdlResult{ 0, 0, m_pTexture->GetWidth(), m_pTexture->GetHeight() };

	//get center point
	SDL_Point centerPoint = originMethod->GetCenterPoint();

	SDL_Rect display_rect = layoutMethod->GetLayoutInformation();
#endif

	//get center point
	auto centerPoint = originMethod->GetCenterPoint();

	if (true == m_bUIChanged)
	{
#ifndef OPENGL_RENDERING
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
			//clone texture
			m_pTextureToRender = m_pTexture->Clone(UIHelper::GetRenderer());
			TextureManipulator(m_pTextureToRender).ColorKey(color);
		}

		//set opacity
		uint8_t alpha = originMethod->GetOpacity();
		if (m_pParentUIObject.lock())
		{
			alpha = originMethod->GetOpacity() * (m_pParentUIObject.lock()->GetPropertyValue<uint8_t>(OPACITY) / 255.0f);
		}
		m_pTextureToRender->setAlpha(alpha);

#endif
		//reset flag
		m_bUIChanged = false;
	}

#ifndef OPENGL_RENDERING
	//<Render
	RenderExContextPtr context = RenderExContext::create(UIHelper::GetRenderer(), sdlResult, display_rect, originMethod->GetAngle(), centerPoint, originMethod->GetFlip());
	context->excute(m_pTextureToRender);
#else
	//< foreground color
	auto foregroundColor = originMethod->GetForeGroundColor();

	//< calculate opacity
	auto opacity = originMethod->GetOpacity();
	if (m_pParentUIObject.lock())
	{
		opacity = originMethod->GetOpacity() * m_pParentUIObject.lock()->GetPropertyValue<float>(OPACITY);
	}

	//<set world matrix
	Renderer3D::GetInstance()->setModalMatrix(m_worldTransform);
	//Render image
	Renderer3D::GetInstance()->DrawImage(m_pTexture, opacity, foregroundColor);
#endif
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
#ifdef OPENGL_RENDERING
	m_pTexture = Library::GetInstance()->get<GLTexture>(texture_name);
#else
	m_pTexture = Library::GetInstance()->get<Texture>(texture_name);

	//clone texture
	m_pTextureToRender = m_pTexture->Clone(UIHelper::GetRenderer());
#endif
	//set flag ui update
	m_bUIChanged = true;
}