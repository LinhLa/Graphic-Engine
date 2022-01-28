#include "stdafx.h"
#include "NodeButton2D.h"
#include "OriginProperty.h"
#include "KeyInputSignalMethod.h"
#include "macro_define.h"
#include "SignalDefine.h"
#include "UIHelper.h"

NodeButton2D::NodeButton2D(std::string name):UIObject(name)
{
	AddSignalMethodObj(KeyInputSignalMethod::create(dynamic_cast<SignalTable*>(this)));
	bind(ON_DRAW_SIGNAL, this, &NodeButton2D::onDraw);
	bind(ON_CLEAN_SIGNAL, this, &NodeButton2D::onClean);
}

NodeButton2D::~NodeButton2D() {}

uint8_t NodeButton2D::getType()
{
	return NODE_2D_BUTTON_TYPE;
}

UIObjectPtr NodeButton2D::clone()
{
	auto pObject = NodeButton2D::create(m_name);
	this->setProperty(pObject);
	return pObject;
}

void NodeButton2D::onInit(VoidType&&)
{}

void NodeButton2D::onDraw(VoidType&&)
{
	auto OriginMethod = GetPropertyMethodObj<OriginProperty>();
	if (!OriginMethod->isVisible())
	{
		return;
	}

	m_ArrayButtonState[m_CurrentSprite]->onDraw();
	m_pCaption->onDraw(VoidType{});
}

void NodeButton2D::onClean(VoidType&&)
{
	//<clean backgournd texture and text texture
	for (auto &p : m_ArrayButtonState)
	{
		p->onClean();
	}
	m_pCaption->onClean(VoidType{});
}

void NodeButton2D::setButtonSprite(BUTTON_SPIRE state, Node2DImagePtr pObject)
{
	m_ArrayButtonState[state] = pObject;
}

void NodeButton2D::setButtonCaption(NodeText2DPtr pObject)
{
	m_pCaption = pObject;
}