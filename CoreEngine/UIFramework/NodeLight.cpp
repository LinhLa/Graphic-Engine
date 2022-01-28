#include "stdafx.h"
#include <base_define.h>
#include <NodeLight.h>
#include "LayoutProperty.h"
#include "MaterialProperty.h"
#include "Material.h"
#include "LightingProperty.h"
#include "SignalDefine.h"

NodePointLight::NodePointLight(std::string name) :UIObject(name)
{
	AddPropertyMethodObj(PointLightProperty::create(dynamic_cast<PropertyTable*>(this)));

	//<on draw
	bind(ON_DRAW_SIGNAL, this, &NodePointLight::onDraw);
}

NodePointLight::~NodePointLight() {}

uint8_t NodePointLight::getType()
{
	return NODE_LIGHT;
}

UIObjectPtr NodePointLight::clone()
{
	auto pObject = NodePointLight::create(m_name);
	this->setProperty(pObject);
	return pObject;
}

void NodePointLight::onInit(VoidType&&)
{}

void NodePointLight::onDraw(VoidType&&)
{
	//calculate light position, direction
	auto layoutProperty = GetPropertyMethodObj<LayoutProperty>();
	
	//<set light position in world
	auto position = glm::vec3(m_worldTransform * glm::vec4(0.0f,0.0f,0.0f,1.0f));
	SetPropertyValue<glm::vec3>(POINT_LIGHT_POSITION, position);
}

void NodePointLight::onClean(VoidType&&)
{}


NodeSpotLight::NodeSpotLight(std::string name) :UIObject(name)
{
	AddPropertyMethodObj(SpotLightProperty::create(dynamic_cast<PropertyTable*>(this)));
	//<on draw
	bind(ON_DRAW_SIGNAL, this, &NodeSpotLight::onDraw);
}

NodeSpotLight::~NodeSpotLight() {}

uint8_t NodeSpotLight::getType()
{
	return NODE_LIGHT;
}

UIObjectPtr NodeSpotLight::clone()
{
	auto pObject = NodeSpotLight::create(m_name);
	this->setProperty(pObject);
	return pObject;
}

void NodeSpotLight::onInit(VoidType&&)
{}

void NodeSpotLight::onDraw(VoidType&&)
{
	//calculate light position, direction
	auto layoutProperty = GetPropertyMethodObj<LayoutProperty>();

	//<set light position in world
	auto position = glm::vec3(m_worldTransform * glm::vec4(layoutProperty->GetLayoutTransform(), 1.0f));
	SetPropertyValue<glm::vec3>(SPOT_LIGHT_POSITION, position);
}

void NodeSpotLight::onClean(VoidType&&)
{}


NodeDirectionalLight::NodeDirectionalLight(std::string name) :UIObject(name)
{
	AddPropertyMethodObj(DirectionalLightProperty::create(dynamic_cast<PropertyTable*>(this)));

	//<on draw
	bind(ON_DRAW_SIGNAL, this, &NodeDirectionalLight::onDraw);
}

NodeDirectionalLight::~NodeDirectionalLight() {}

uint8_t NodeDirectionalLight::getType()
{
	return NODE_LIGHT;
}

UIObjectPtr NodeDirectionalLight::clone()
{
	auto pObject = NodeDirectionalLight::create(m_name);
	this->setProperty(pObject);
	return pObject;
}

void NodeDirectionalLight::onInit(VoidType&&)
{}

void NodeDirectionalLight::onDraw(VoidType&&)
{
	//calculate light direction to world tranform
	auto layoutProperty = GetPropertyMethodObj<LayoutProperty>();
}

void NodeDirectionalLight::onClean(VoidType&&)
{}