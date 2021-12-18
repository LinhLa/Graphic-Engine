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

#include "GLRenderContext.h"
#include "Renderer3D.h"

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

	//set flag ui update on alpha changed
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
#ifndef OPENGL_RENDERING
	SDL_Rect sdlResult{0, 0, m_pTexture->GetWidth(), m_pTexture->GetHeight()};
#endif
	SDL_Rect display_rect = layoutMethod->GetLayoutInformation();

	//get center point
	SDL_Point centerPoint  = originMethod->GetCenterPoint();

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
			if (IS_VALID_COLOR(color))
			{

				//clone texture
				m_pTextureToRender = m_pTexture->Clone(UIHelper::GetRenderer());
				TextureManipulator(m_pTextureToRender).ColorKey(color);
			}
		}

		//set opacity
		m_pTextureToRender->setAlpha(originMethod->GetOpacity());

#endif
		//reset flag
		m_bUIChanged = false;
	}

#ifndef OPENGL_RENDERING
	//<Render
	RenderExContextPtr context = RenderExContext::create(UIHelper::GetRenderer(), sdlResult, display_rect, originMethod->GetAngle(), centerPoint, originMethod->GetFlip());
	context->excute(m_pTextureToRender);
#else

	//get foreground color
	SDL_Color color = originMethod->GetForeGroundColor();
	glm::vec4 foregroundColor = glm::vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	if (!IS_VALID_COLOR(color))
	{
		foregroundColor = glm::vec4(1.0f);
	}

	//<Render
	auto context = GLRender2DContext::create(
		m_pTexture,
		glm::vec2(static_cast<float>(display_rect.x), static_cast<float>(display_rect.y)),
		glm::vec2(layoutMethod->GetLayoutScaleX(), layoutMethod->GetLayoutScaleY()),
		foregroundColor,
		static_cast<float>(originMethod->GetAngle()),
		originMethod->GetOpacity() / 255.0f,
		glm::vec2(centerPoint.x, centerPoint.y),
		SDL_FLIP_NONE);

	context->excute();

	//<set world matrix, view matrix, projection matrix
	Renderer3D::GetInstance()->setModalMatrix(m_worldTransform);
	Renderer3D::GetInstance()->setViewMatrix(glm::mat4());
	Renderer3D::GetInstance()->setProjectionMatrix(pCamera->projectionMatrix());
	//Render Geometry
	Renderer3D::GetInstance()->DrawImage(m_pTexture, m_pMaterial, m_pModel);
	Renderer3D::DrawImage(GLTexturePtr pTexture, glm::vec2 coordinator, glm::vec2 scale, float angle, glm::vec2 origin, float opacity, glm::vec4 color)
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