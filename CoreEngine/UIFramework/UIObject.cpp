#include "stdafx.h"
#include <algorithm>
#include <cmath>
#include "UIObject.h"
#include "Configuration.h"
#include "Render.h"
#include "Renderer3D.h"

#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "GLProperty.h"

#include "KeyInputSignalMethod.h"
#include "UIObjectSignalMethod.h"

#include "PropertyDefine.h"
#include "SignalDefine.h"

#include "RenderManipulator.h"
#include "GLRenderManipulator.h"

#include "UIHelper.h"
#include "Library.h"

using namespace UIHelper;

UIObject::UIObject(std::string name) :m_name(name)
{
#ifdef OPENGL_RENDERING
	AddPropertyMethodObj(GLProperty::create(dynamic_cast<PropertyTable*>(this)));
#endif
	AddPropertyMethodObj(OriginProperty::create(dynamic_cast<PropertyTable*>(this)));
	AddPropertyMethodObj(LayoutProperty::create(dynamic_cast<PropertyTable*>(this)));

	AddSignalMethodObj(KeyInputSignalMethod::create(dynamic_cast<SignalTable*>(this)));
	AddSignalMethodObj(UIObjectSignalMethod::create(dynamic_cast<SignalTable*>(this)));
}

UIObject::~UIObject()
{}

void UIObject::onInit()
{
	//<broadcast signal on init
	OnSignal(ON_INIT_SIGNAL, VoidType{});

	for (auto &child : m_childList)
	{
		child->onInit();
	}
}

void UIObject::onDraw()
{
	auto originMethod = GetPropertyMethodObj<OriginProperty>();
	auto LayoutMethod = GetPropertyMethodObj<LayoutProperty>();
	//<Stop draw on invisible
	if (!originMethod->isVisible())
	{
		return;
	}

	SDL_Rect parent_rect = LayoutMethod->GetLayoutInformation();
#ifndef OPENGL_RENDERING
	RenderClipManipulator clipManipulator(UIHelper::GetRenderer(), parent_rect);
#else
	GLRenderClipManipulator clipManipulator(parent_rect);
#endif

	//<Check clip area
	if (originMethod->IsClip())
	{
		//Ignore if clip area is not valid
		if (!clipManipulator.HasIntersection())
		{
			return;
		}
		clipManipulator.SetRenderClipTarget();
	}

	//Check background color
	if (IsPropertyExist(BACK_GROUND_COLOR))
	{
		SDL_Color color = originMethod->GetBackGroundColor();
		if (IS_VALID_COLOR(color))
	 	{
	 		//Calculate Alpha
	 		color.a = static_cast<uint8_t>((originMethod->GetOpacity() / 255.0F) * color.a);
		#ifndef OPENGL_RENDERING
	 		//Create render manip
			RenderDrawManipulator drawer(UIHelper::GetRenderer(), SDL_BLENDMODE_BLEND, color);

			//Fill color
			drawer.FillRect(parent_rect);
		#else
			glm::vec3 scale = LayoutMethod->GetLayoutScale();
			Renderer3D::GetInstance()->DrawColor(
				glm::vec2(static_cast<float>(parent_rect.x), static_cast<float>(parent_rect.y)),
				glm::vec2(static_cast<float>(parent_rect.w), static_cast<float>(parent_rect.h)),
				glm::vec2(scale.x, scale.y),
				static_cast<float>(originMethod->GetAngle()),
				glm::vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f));
		#endif
		}
	}

	//<broadcast signal on draw
	OnSignal(ON_DRAW_SIGNAL, VoidType{});

	//<Draw child list
	for (auto &child : m_childList)
	{
		//draw
		child->onDraw();
	}
}

void UIObject::onClean()
{
	//<broadcast signal on clean
	OnSignal(ON_CLEAN_SIGNAL, VoidType{});

	for (auto &child : m_childList)
	{
		child->onClean();
	}
}

void UIObject::onKeyInputEvent(SDL_Event& arg)
{
	auto originMethod = this->GetPropertyMethodObj<OriginProperty>();
	auto LayoutMethod = this->GetPropertyMethodObj<LayoutProperty>();

	SDL_Rect display_rect = LayoutMethod->GetLayoutInformation();
	UIHelper::MOUSE_STATE state = UIHelper::onMouseEvent(arg, display_rect);

	//Get mouse position
	int  x, y;
	SDL_GetMouseState(&x, &y);

	//Apply render scale to mouse position
	x = static_cast<int>(static_cast<float>(x) / WindowRender::GetInstance()->scaleX());
	y = static_cast<int>(static_cast<float>(y) / WindowRender::GetInstance()->scaleY());

	//Trigger mouse event
	switch (state)
	{
	case MOUSE_STATE::MOUSE_DOWN:
		OnSignal(MOUSE_DOWN_SIGNAL, SDL_Point{ x,y });
		if (!m_bleftMouseButtonDown && SDL_BUTTON_LEFT == arg.button.button)
		{
			LOG_OFF("Node name[%s]: Mouse Down", m_name.c_str());
			m_bleftMouseButtonDown = true;
		}
		break;
	case MOUSE_STATE::MOUSE_OUT:
		LOG_OFF("Node name[%s]: Mouse Out", m_name.c_str());
		OnSignal(MOUSE_OUT_SIGNAL, SDL_Point{ x,y });
		m_bleftMouseButtonDown = false;
		break;
	case MOUSE_STATE::MOUSE_OVER_MOTION:
		if (m_bleftMouseButtonDown)
		{
			LOG_OFF("Node name[%s]: Mouse Drag", m_name.c_str());
			OnSignal(MOUSE_DRAG_SIGNAL, SDL_Point{ x,y });
		}
		else
		{
			LOG_OFF("Node name[%s]: Mouse Move", m_name.c_str());
			OnSignal(MOUSE_OVER_MOTION_SIGNAL, SDL_Point{ x,y });
		}
		break;
	case MOUSE_STATE::MOUSE_UP:
		if (m_bleftMouseButtonDown && SDL_BUTTON_LEFT == arg.button.button)
		{
			m_bleftMouseButtonDown = false;
		}
		LOG_OFF("Node name[%s]: Mouse Up", m_name.c_str());
		OnSignal(MOUSE_UP_SIGNAL, SDL_Point{ x,y });
		break;
	default:
		break;
	}

	//Trigger key press event
	if (SDL_KEYDOWN == arg.type || SDL_KEYUP == arg.type)
	{
		LOG_OFF("Node name[%s]: Key Pressed", m_name.c_str());
		OnSignal(KEY_PRESS_SIGNAL, std::move(uint32_t{ arg.type }), static_cast<uint32_t>(arg.key.keysym.sym));
	}

	//Broadcast event to child list if enable
	if (originMethod->IsBroadCastEvent())
	{
		for (auto &child : m_childList)
		{
			child->onKeyInputEvent(arg);
		}
	}
}

std::string UIObject::getUrl() const
{
	if (!m_pParentUIObject)
	{
		return std::string();
	}
	return m_pParentUIObject->getUrl() + std::string("/") + m_name;
}

std::string UIObject::getName() const
{
	return m_name;
}

void UIObject::moveTo(UIObjectPtr parent)
{
	auto result = m_pParentUIObject->getChild(m_name);
	if (result && parent && m_pParentUIObject)
	{
		parent->addChild(result);
		m_pParentUIObject = parent;
		m_pParentUIObject->removeChild(m_name);
	}
}

void UIObject::addChild(UIObjectPtr child)
{
	if (child)
	{
		m_childList.push_back(child);
	}
}

void UIObject::removeChild(std::string m_name)
{
	for (auto itr = m_childList.begin(); itr != m_childList.end(); ++itr)
	{
		if ((*itr)->getName() == m_name)
		{
			m_childList.erase(itr);
			return;
		}
	}
}

UIObjectPtr UIObject::getChild(std::string m_name)
{
	for (auto &child : m_childList)
	{
		if (child->getName() == m_name)
		{
			return child;
		}
	}
	return nullptr;
}