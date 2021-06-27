#include "stdafx.h"

#include <base_define.h>
#include <Node3D.h>
#include "Library.h"

#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "GLProperty.h"

#include "UIHelper.h"
#include "GLRenderContext.h"

#include "PropertyDefine.h"
#include "SignalDefine.h"

Node3D::Node3D(std::string name):UIObject(name)
{
	//<on draw
	bind(ON_DRAW_SIGNAL, this, &Node3D::onDraw);
}

Node3D::~Node3D() {}

uint8_t Node3D::getType()
{
	return NODE_3D;
}

void Node3D::onInit(VoidType&&)
{}

void Node3D::onDraw(VoidType&&)
{
	auto originMethod = GetPropertyMethodObj<OriginProperty>();
	auto layoutMethod = GetPropertyMethodObj<LayoutProperty>();
	auto glProperty = GetPropertyMethodObj<GLProperty>();

	m_pShaderProgram->setUniformFromUIObject(shared_from_this());

	//<Create render context
	/*auto context = GLRender3DContext::create(
		layoutMethod->GetLayoutScale(),
		layoutMethod->GetLayoutTransform(),
		layoutMethod->GetRotation(),
		originMethod->GetAngle(),
		glProperty->GetTextureList(),
		m_pShaderProgram,
		m_pMesh);*/
	
	//auto pTexture = Library::GetInstance()->get<GLTexture>("landscape.png");
	SDL_Rect clip;
	SDL_Rect display{ 0,0, 0, 0};
	double angle = 0.0f;
	SDL_Point center{0,0};
	auto context = GLRender2DContext::create(
		m_pShaderProgram,
		m_pMesh,
		glProperty->GetTextureList(),
		clip,
		display,
		angle,
		center,
		SDL_FLIP_NONE);
	context->excute();
}

void Node3D::onClean(VoidType&&)
{}

float Node3D::GetActualWidth() const
{
	return GetPropertyValue<float>(LAYOUT_WIDTH);
}

float Node3D::GetActualHeight() const
{
	return GetPropertyValue<float>(LAYOUT_HEIGHT);
}

void Node3D::SetMesh(const std::string& name)
{
	m_pMesh = Library::GetInstance()->get<Mesh>(name);
}

void Node3D::SetProgram(const std::string& name)
{
	m_pShaderProgram = Library::GetInstance()->get<ShaderProgram>(name);
	m_pShaderProgram->syncUniformToUIObject(shared_from_this());
}