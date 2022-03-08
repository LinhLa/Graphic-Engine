#include "stdafx.h"

#include <base_define.h>
#include <Node3D.h>
#include <NodeMesh.h>
#include <NodeCamera.h>
#include "Library.h"

#include "OriginProperty.h"
#include "LayoutProperty.h"
#include "GLProperty.h"
#include "MaterialProperty.h"
#include "CameraProperty.h"

#include "UIHelper.h"

#include "PropertyDefine.h"
#include "SignalDefine.h"

#include "Camera.h"
#include "Renderer3D.h"

#include "GLRenderContext.h"
#include "GLRenderManipulator.h"

Node3D::Node3D(std::string name) :UIObject(name)
{
	//<create default camera
	m_camera = NodeCamera::create(name);

	//<on draw
	bind(ON_DRAW_SIGNAL, this, &Node3D::onDraw);

	//<on draw done
	bind(ON_DRAW_DONE_SIGNAL, this, &Node3D::onDrawDone);
}

Node3D::~Node3D() {}

uint8_t Node3D::getType()
{
	return NODE_3D;
}

UIObjectPtr Node3D::clone()
{
	auto pObject = Node3D::create(m_name);
	this->setProperty(pObject);
	for (auto& model : m_modelStack)
	{
		pObject->SetModel(model.second->getName());
	}
	pObject->SetNodeLight<NodePointLight>(m_pointLights);
	pObject->SetNodeLight<NodeSpotLight>(m_spotLights);
	pObject->SetNodeLight<NodeDirectionalLight>(m_directionalLights);
	pObject->SetNodeCamera(m_camera);
	return pObject;
}

void Node3D::onInit(VoidType&&)
{}

void Node3D::onDraw(VoidType&&)
{
	auto glProperty = GetPropertyMethodObj<GLProperty>();
	auto cameraProperty = m_camera->GetPropertyMethodObj<CameraProperty>();
	//<view matrix, projection matrix
	auto pCamera = Camera::create(cameraProperty);

	Renderer3D::GetInstance()->setViewMatrix(pCamera->View());
	Renderer3D::GetInstance()->setProjectionMatrix(pCamera->projectionMatrix());

	//<set view port
	GLRenderViewPortManipulator::push(glProperty->GetViewPort());
}

void Node3D::onDrawDone(VoidType&&)
{
	//<pop view port
	GLRenderViewPortManipulator::pop();
}

void Node3D::onClean(VoidType&&)
{}

void Node3D::SetModel(const std::string& name)
{
	auto pModel = Library::GetInstance()->get<Model>(name);
	if (pModel)
	{
		m_modelStack[pModel->getName()] = pModel;
		auto meshes = pModel->getMeshes();
		for (auto& mesh : meshes)
		{
			auto node = NodeMesh::create(mesh->getName());
			node->SetMesh(mesh);
			addChild(node);
		}
	}
}

void Node3D::SetNodeCamera(NodeCameraPtr camera)
{
	m_camera = camera;
}