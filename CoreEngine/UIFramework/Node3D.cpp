#include "stdafx.h"

#include <base_define.h>
#include <Node3D.h>
#include "Library.h"

#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "GLProperty.h"
#include "MaterialProperty.h"

#include "UIHelper.h"
#include "GLRenderContext.h"

#include "PropertyDefine.h"
#include "SignalDefine.h"

#include "Camera.h"
#include "Renderer3D.h"

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
	auto originMethod	= GetPropertyMethodObj<OriginProperty>();
	auto layoutMethod	= GetPropertyMethodObj<LayoutProperty>();
	auto glProperty		= GetPropertyMethodObj<GLProperty>();
	auto materialProperty = GetPropertyMethodObj<MaterialProperty>();
	//update uniform
	m_pShaderProgram->setUniform(m_pMaterial);

	//get layout display
	SDL_Rect display_rect = layoutMethod->GetLayoutInformation();

	//get pivot point
	glm::vec3 pivotPoint = originMethod->GetPivotPoint();

	//<Create render context
	/*auto context = GLRender3DContext::create(
		layoutMethod->GetLayoutScale(),
		layoutMethod->GetLayoutTransform(),
		layoutMethod->GetRotation(),
		originMethod->GetAngle(),
		m_pShaderProgram,
		m_pMaterial,
		m_pModel,
		Camera::create(glProperty));
	
	context->excute();*/

	//<set world matrix, view matrix, projection matrix
	auto pCamera = Camera::create(glProperty);
	Renderer3D::GetInstance()->setModalMatrix(m_worldTransform);
	Renderer3D::GetInstance()->setViewMatrix(pCamera->View());
	Renderer3D::GetInstance()->setProjectionMatrix(pCamera->projectionMatrix());
	//Render Geometry
	Renderer3D::GetInstance()->DrawGeometry(m_pShaderProgram, m_pMaterial, m_pModel);
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

void Node3D::SetProgram(const std::string& name)
{
	m_pShaderProgram = Library::GetInstance()->get<ShaderProgram>(name);
}

void Node3D::SetModel(const std::string& name)
{
	m_pModel = Library::GetInstance()->get<Model>(name);
}

void Node3D::SetMaterial(const std::string& name)
{
	m_pMaterial = Library::GetInstance()->get<Material>(name);
	m_pShaderProgram->syncUniform(m_pMaterial);
}