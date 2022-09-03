#include "stdafx.h"
#include <base_define.h>
#include <NodeCamera.h>
#include "CameraProperty.h"
#include "SignalDefine.h"

NodeCamera::NodeCamera(std::string name) :UIObject(name)
{
	AddPropertyMethodObj(CameraProperty::create(dynamic_cast<PropertyTable*>(this)));

	//<on draw
	bind(ON_DRAW_SIGNAL, this, &NodeCamera::onDraw);
}

NodeCamera::~NodeCamera() {}

uint8_t NodeCamera::getType()
{
	return NODE_LIGHT;
}

UIObjectPtr NodeCamera::clone()
{
	auto pObject = NodeCamera::create(m_name);
	this->setProperty(pObject);
	return pObject;
}

void NodeCamera::onInit(VoidType&&)
{}

void NodeCamera::onDraw(VoidType&&)
{
}

void NodeCamera::onClean(VoidType&&)
{}