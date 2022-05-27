#include "stdafx.h"
#include <algorithm>
#include <cmath>
#include "UIObject.h"
#include "Configuration.h"
#include "Render.h"
#include "WindowRender.h"
#include "Renderer3D.h"

#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "GLProperty.h"
#include "MaterialProperty.h"

#include "KeyInputSignalMethod.h"
#include "UIObjectSignalMethod.h"

#include "PropertyDefine.h"
#include "SignalDefine.h"


#include "RenderManipulator.h"
#include "GLRenderManipulator.h"


#include "UIHelper.h"
#include "Library.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp >

using namespace UIHelper;

extern const std::map<std::string, uint8_t> gMapNodeType =
{
	{"2D_IMAGE", NODE_2D_IMAGE_TYPE},
	{"2D_BUTTON", NODE_2D_BUTTON_TYPE},
	{"2D_TEXT", NODE_2D_TEXT_TYPE},
	{"EMPTY_NODE", EMPTY_NODE},
	{"STACK_LAYOUT_2D", STACK_LAYOUT_2D},
	{"NODE_3D", NODE_3D},
	{"NODE_MESH", NODE_MESH},
	{"NODE_LIGHT", NODE_LIGHT}
};

UIObject::UIObject(std::string name) :m_name(name)
{
#ifdef OPENGL_RENDERING
	AddPropertyMethodObj(GLProperty::create(dynamic_cast<PropertyTable*>(this)));
#endif
	AddPropertyMethodObj(LayoutProperty::create(dynamic_cast<PropertyTable*>(this)));
	AddPropertyMethodObj(OriginProperty::create(dynamic_cast<PropertyTable*>(this)));

	AddSignalMethodObj(KeyInputSignalMethod::create(dynamic_cast<SignalTable*>(this)));
	AddSignalMethodObj(UIObjectSignalMethod::create(dynamic_cast<SignalTable*>(this)));
}

UIObject::~UIObject()
{}

void UIObject::onInit()
{
	//<broadcast signal on init
	OnSignal(ON_INIT_SIGNAL, VoidType{});

	for (auto& child : m_childList)
	{
		child->onInit();
	}
}

#ifdef OPENGL_RENDERING
std::unique_ptr<GLRenderClipManipulator> UIObject::updateClipArea()
{
	auto originMethod = GetPropertyMethodObj<OriginProperty>();
	auto LayoutMethod = GetPropertyMethodObj<LayoutProperty>();
	//<Set clip area
	auto size = LayoutMethod->GetLayoutSize();
	auto transform = LayoutMethod->GetLayoutTransform();
	auto scaleX = WindowRender::GetInstance()->scaleX();
	auto scaleY = WindowRender::GetInstance()->scaleY();
	glm::vec4 parent_rect(transform.x * scaleX, transform.y * scaleY, size.x * scaleX, size.y * scaleY);
	std::unique_ptr<GLRenderClipManipulator> pClipManipulator(new GLRenderClipManipulator(parent_rect));

	//Ignore if clip area is not valid
	if (!pClipManipulator->HasIntersection())
	{
		return nullptr;
	}
	pClipManipulator->SetRenderClipTarget();
	return std::move(pClipManipulator);
}

#else

std::unique_ptr<RenderClipManipulator> UIObject::updateClipArea()
{
	auto originMethod = GetPropertyMethodObj<OriginProperty>();
	auto LayoutMethod = GetPropertyMethodObj<LayoutProperty>();
	SDL_Rect parent_rect = LayoutMethod->GetLayoutInformation();
	std::unique_ptr<RenderClipManipulator> pClipManipulator(new RenderClipManipulator(UIHelper::GetRenderer(), parent_rect));

	//Ignore if clip area is not valid
	if (!pClipManipulator->HasIntersection())
	{
		return nullptr;
	}
	pClipManipulator->SetRenderClipTarget();
	return std::move(pClipManipulator);
}
#endif

void UIObject::updateBackground()
{
	auto originMethod = GetPropertyMethodObj<OriginProperty>();
	auto LayoutMethod = GetPropertyMethodObj<LayoutProperty>();
	if (IsPropertyExist(BACK_GROUND_COLOR))
	{
#ifndef OPENGL_RENDERING
		SDL_Color color = originMethod->GetBackGroundColor();
		if (OriginProperty::isValidColor(color))
		{
			//Calculate Alpha
			color.a = static_cast<uint8_t>((originMethod->GetOpacity() / 255.0F) * color.a);
			if (m_pParentUIObject.lock())
			{
				color.a = ((originMethod->GetOpacity() / 255.0F) * (m_pParentUIObject.lock()->GetPropertyValue<uint8_t>(OPACITY) / 255.0f)) * color.a;
			}

			//Create render manip
			RenderDrawManipulator drawer(UIHelper::GetRenderer(), SDL_BLENDMODE_BLEND, color);

			//Fill color
			drawer.FillRect(parent_rect);
		}
#else
		auto color = originMethod->GetBackGroundColor();
		if (OriginProperty::isValidColor(color))
		{
			// calculate Alpha
			if (m_pParentUIObject.lock())
			{
				color.a *= (originMethod->GetOpacity() * m_pParentUIObject.lock()->GetPropertyValue<float>(OPACITY));
			}
			Renderer3D::GetInstance()->DrawColor(LayoutMethod->GetLayoutSize(), color);
		}
#endif
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
#ifdef OPENGL_RENDERING
	std::unique_ptr<GLRenderClipManipulator> pClipManip = nullptr;
#else
	std::unique_ptr<RenderClipManipulator> pClipManip = nullptr;
#endif
	//<Check clip area
	if (originMethod->IsClip())
	{
		pClipManip = std::move(updateClipArea());
		if (!pClipManip)
		{
			return;
		}
}


	//update transform matrix
	updateWorldTransform();

	//Check background color
	updateBackground();

	//<broadcast signal on draw
	OnSignal(ON_DRAW_SIGNAL, VoidType{});

	//<Draw child list
	for (auto& child : m_childList)
	{
		//draw
		child->onDraw();
	}

	//<broadcast signal after draw
	OnSignal(ON_DRAW_DONE_SIGNAL, VoidType{});
}

void UIObject::onClean()
{
	//<broadcast signal on clean
	OnSignal(ON_CLEAN_SIGNAL, VoidType{});

	for (auto& child : m_childList)
	{
		child->onClean();
	}
}

void UIObject::onKeyInputEvent(SDL_Event& arg)
{
	auto originMethod = this->GetPropertyMethodObj<OriginProperty>();
	auto LayoutMethod = this->GetPropertyMethodObj<LayoutProperty>();
#ifdef OPENGL_RENDERING
	auto pos = LayoutMethod->GetLayoutTransform();
	auto size = LayoutMethod->GetLayoutSize();
	SDL_Rect display_rect = { pos.x, pos.y, size.x, size.y };
#else
	SDL_Rect display_rect = LayoutMethod->GetLayoutInformation();
#endif

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
		for (auto& child : m_childList)
		{
			child->onKeyInputEvent(arg);
		}
	}
}

std::string UIObject::getUrl() const
{
	auto parent = m_pParentUIObject.lock();
	if (!parent)
	{
		return m_name;
	}
	return parent->getUrl() + std::string("/") + m_name;
}

std::string UIObject::getName() const
{
	return m_name;
}

void UIObject::moveTo(UIObjectPtr parent)
{
	if (parent)
	{
		auto current_parent = m_pParentUIObject.lock();
		if (current_parent)
		{
			current_parent->removeChild(m_name);
		}
		else
		{
		}
		parent->addChild(shared_from_this());
		m_pParentUIObject = parent;
	}
	else {}
}
void UIObject::setParent(UIObjectPtr parent)
{
	if (parent)
	{
		m_pParentUIObject = parent;
	}
}

void UIObject::addChild(UIObjectPtr child)
{
	if (child)
	{
		m_childList.push_back(child);
		child->setParent(shared_from_this());
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

#ifdef OPENGL_RENDERING
void UIObject::updateWorldTransform()
{
	auto parent = this->m_pParentUIObject.lock();
	this->updateLocalTransform();
	if (parent != nullptr)
	{
		m_worldTransform = parent->worldTransform() * m_localTransform;
	}
	else
	{
		m_worldTransform = m_localTransform;
	}
	LOG_OFF("Node[%s] Transform: %s", m_name.c_str(), glm::to_string(m_worldTransform).c_str());
}
void UIObject::updateLocalTransform()
{
	switch (getType())
	{
	case NODE_2D_IMAGE_TYPE:
	case NODE_2D_BUTTON_TYPE:
	case NODE_2D_TEXT_TYPE:
	case EMPTY_NODE:
	case STACK_LAYOUT_2D:
	{
		updateLocalTransform2D();
		break;
	}
	case NODE_3D:
	case NODE_MESH:
	case NODE_LIGHT:
	{
		updateLocalTransform3D();
		break;
	}
	default:
		break;
	}
}

void UIObject::updateLocalTransform2D()
{
	auto layoutMethod = GetPropertyMethodObj<LayoutProperty>();
	auto originMethod = GetPropertyMethodObj<OriginProperty>();
	auto size = layoutMethod->GetLayoutSize();
	auto angle = originMethod->GetAngle();
	auto scale = layoutMethod->GetLayoutScale();
	auto position = layoutMethod->GetLayoutTransform();
	auto center_point = originMethod->GetCenterPoint();
	auto transform = glm::mat4(1.0f);

	auto alignment_verizontal = originMethod->GetAlignVerizontal();
	auto alignment_horizontal = originMethod->GetAlignHorizontal();

	glm::vec3 oscillation = getOscillation(alignment_verizontal, alignment_horizontal, size, scale);

	transform = glm::translate(transform, position + oscillation);
	transform = glm::rotate(transform, glm::radians(angle), glm::vec3(center_point.x, center_point.y, 1.0));
	transform = glm::scale(transform, scale);
	m_localTransform = transform;
}

void UIObject::updateLocalTransform3D()
{
	auto layoutMethod = GetPropertyMethodObj<LayoutProperty>();
	auto originMethod = GetPropertyMethodObj<OriginProperty>();

	auto pivot_point = originMethod->GetPivotPoint();
	auto scale = layoutMethod->GetLayoutScale();
	auto rotation = layoutMethod->GetRotation();
	auto position = layoutMethod->GetLayoutTransform();
	auto transform = glm::mat4(1.0f);
	transform = glm::translate(transform, position);

	//Rotation use Euler angle
	//transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	//transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	//transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

	//Rotaion use Quaternion
	transform *= glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));

	transform = glm::scale(transform, scale);
	m_localTransform = transform;
}

glm::mat4 UIObject::worldTransform() const
{
	return m_worldTransform;
}

glm::vec3 UIObject::getOscillation(int alignment_verizontal, int alignment_horizontal, glm::vec2 size, glm::vec3 scale)
{
	std::map<int, glm::vec3> sMapOscillation =
	{
		{ALGIN_CENTER + ALGIN_CENTER, glm::vec3((size.x - (size.x * scale.x)) / 2.0,(size.y - (size.y * scale.y)) / 2.0,0.0f)},
		{ALGIN_TOP + ALGIN_LEFT, glm::vec3(0.0f)},
		{ALGIN_TOP + ALGIN_CENTER, glm::vec3((size.x - (size.x * scale.x)) / 2.0, 0.0f, 0.0f)},
		{ALGIN_TOP + ALGIN_RIGHT, glm::vec3(size.x - (size.x * scale.x), 0.0f, 0.0f)},
		{ALGIN_CENTER + ALGIN_LEFT, glm::vec3(0.0f, (size.y - (size.y * scale.y)) / 2.0, 0.0f)},
		{ALGIN_CENTER + ALGIN_RIGHT, glm::vec3(size.x - (size.x * scale.x), (size.y - (size.y * scale.y)) / 2.0, 0.0f)},
		{ALGIN_BOTTOM + ALGIN_LEFT, glm::vec3(0.0f, size.y - (size.y * scale.y), 0.0f)},
		{ALGIN_BOTTOM + ALGIN_CENTER, glm::vec3((size.x - (size.x * scale.x)) / 2.0, size.y - (size.y * scale.y), 0.0f)},
		{ALGIN_BOTTOM + ALGIN_RIGHT, glm::vec3(size.x - (size.x * scale.x), size.y - (size.y * scale.y), 0.0f)}
	};
	return sMapOscillation.at(alignment_verizontal + alignment_horizontal);
}
#endif