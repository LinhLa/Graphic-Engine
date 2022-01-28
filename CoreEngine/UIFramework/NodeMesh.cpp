#include "stdafx.h"

#include <base_define.h>
#include <Node3D.h>
#include <NodeMesh.h>
#include "Library.h"

#include "Material.h"
#include "MaterialProperty.h"
#include "LightingProperty.h"

#include "UIHelper.h"
#include "GLRenderContext.h"
#include "GLRenderManipulator.h"

#include "PropertyDefine.h"
#include "SignalDefine.h"

#include "Renderer3D.h"

#include "Configuration.h"
#include "DynamicShader.h"

#include <utils.h>
#include <regex>

NodeMesh::NodeMesh(std::string name) :UIObject(name)
{
	AddPropertyMethodObj(MaterialProperty::create(this));
	
	bind(ON_DRAW_SIGNAL, this, &NodeMesh::onDraw);
	bind(ON_INIT_SIGNAL, this, &NodeMesh::onInit);
}

NodeMesh::~NodeMesh() {}

uint8_t NodeMesh::getType()
{
	return NODE_MESH;
}

UIObjectPtr NodeMesh::clone()
{
	auto pObject = NodeMesh::create(m_name);
	pObject->SetMesh(m_pMesh);
	this->setProperty(pObject);
	return pObject;
}

void NodeMesh::onInit(VoidType&&)
{}

void NodeMesh::onDraw(VoidType&&)
{
	//<set world matrix
	Renderer3D::GetInstance()->setModalMatrix(m_worldTransform);

	auto parent = std::dynamic_pointer_cast<Node3D>(m_pParentUIObject.lock());
	auto render_pass = std::dynamic_pointer_cast<Property<GLint>>(parent->GetProperty(RENDER_PASS))->GetValue();
	if (0 != (render_pass & GLRenderStackManipulator::top()))
	{
		//if (SHADOWN_MAP_RENDER_PASS == (render_pass & SHADOWN_MAP_RENDER_PASS))
		//{
		//	//draw scene
		//	auto pMaterial = Library::GetInstance()->get<Material>(SHADOWN_MATERIAL);
		//	auto pShader = Library::GetInstance()->get<ShaderProgram>(SHADOWN_PROGRAM);
		//		Renderer3D::GetInstance()->DrawGeometry(pShader, pMaterial, m_pMesh);
		//	}
		//}
		//else
		{
			/*
			* Work flow:
			1.Sync lighting property to material
			2.Prepare dynamic shader from material
			3.Sync uniform to material
			4.Sync material property to node mesh
			5.Loop:
				- set property from node to material
				- update render
			*/
			m_pShaderProgram = prepareDynamicShader();

			//update material property
			this->setProperty(m_pMaterial);

			//Render Geometry
			Renderer3D::GetInstance()->DrawGeometry(m_pShaderProgram, m_pMaterial, m_pMesh);
		}
	}

}

void NodeMesh::onClean(VoidType&&)
{}

void NodeMesh::SetMesh(MeshPtr pMesh)
{
	m_pMesh = pMesh;
	SetMaterial(m_pMesh->getMaterial());
	SetProgram(m_pMaterial->getShader());
}

void NodeMesh::SetProgram(const std::string& name)
{
	if (Library::GetInstance()->get<ShaderProgram>(name))
	{
		m_pShaderProgram = Library::GetInstance()->get<ShaderProgram>(name);
	}
	else if (Library::GetInstance()->get<Material>(SHADER_DEFAULT))
	{
		m_pShaderProgram = Library::GetInstance()->get<ShaderProgram>(SHADER_DEFAULT);
	}
	else 
	{
		LOG_ERROR("Shader[%s] not found", name.c_str());
		_ASSERT(false);
	}
}

void NodeMesh::SetMaterial(const std::string& name)
{
	if (Library::GetInstance()->get<Material>(name))
	{
		m_pMaterial = Library::GetInstance()->get<Material>(name)->clone();
	}
	else if(Library::GetInstance()->get<Material>(MATERIAL_DEFAULT))
	{
		m_pMaterial = Library::GetInstance()->get<Material>(MATERIAL_DEFAULT)->clone();
	}
	else
	{
		LOG_ERROR("Material[%s] not found", name.c_str());
		_ASSERT(false);
	}
}

MeshPtr NodeMesh::getMesh()
{
	return m_pMesh;
}

MaterialPtr NodeMesh::getMaterial()
{
	return m_pMaterial;
}
ShaderProgramPtr NodeMesh::prepareDynamicShader()
{
	auto parent = std::dynamic_pointer_cast<Node3D>(m_pParentUIObject.lock());
	auto spotLights = parent->getNodeLight<NodeSpotLight>();
	auto pointLights = parent->getNodeLight<NodePointLight>();
	auto directionalLights = parent->getNodeLight<NodeDirectionalLight>();
	for (GLint spotlight = 0; spotlight < spotLights.size(); ++spotlight)
	{
		auto lightPropertyMethod = spotLights[spotlight]->GetPropertyMethodObj<SpotLightProperty>();
		lightPropertyMethod->SetProperty(m_pMaterial, spotlight);
		//calculate direction
		auto direction = glm::vec3(spotLights[spotlight]->worldTransform() * glm::vec4(spotLights[spotlight]->GetPropertyValue<glm::vec3>(SPOT_LIGHT_DIRECTION), 0.0f));
		m_pMaterial->SetPropertyValue<glm::vec3>(parse_name(spotlight, SPOT_LIGHT_DIRECTION), std::move(direction));
	}

	for (GLint pointlight = 0; pointlight < pointLights.size(); ++pointlight)
	{
		auto lightPropertyMethod = pointLights[pointlight]->GetPropertyMethodObj<PointLightProperty>();
		lightPropertyMethod->SetProperty(m_pMaterial, pointlight);
	}

	for (GLint directlight = 0; directlight < directionalLights.size(); ++directlight)
	{
		auto lightPropertyMethod = directionalLights[directlight]->GetPropertyMethodObj<DirectionalLightProperty>();
		lightPropertyMethod->SetProperty(m_pMaterial, directlight);

		//calculate direction
		auto direction = glm::vec3(directionalLights[directlight]->worldTransform() * glm::vec4(directionalLights[directlight]->GetPropertyValue<glm::vec3>(DIRECTIONAL_LIGHT_DIRECTION), 0.0f));
		m_pMaterial->SetPropertyValue<glm::vec3>(parse_name(directlight,DIRECTIONAL_LIGHT_DIRECTION), std::move(direction));
	}

	auto SpotLightCount = std::to_string(spotLights.size());
	auto DirectLightCount = std::to_string(directionalLights.size());
	auto PointLightCount = std::to_string(pointLights.size());
	auto name = LIGHTING_SHADER + m_pMaterial->getName() + SpotLightCount + DirectLightCount + PointLightCount;
	auto pShaderProgram = Library::GetInstance()->get<ShaderProgram>(name);
	if (!pShaderProgram)
	{
		//<load vertex + fragment shader
		std::string fragment_shader_path = Configuration::GetInstance()->shader_path + std::string("LightShadingFrag.glsl");
		std::string vertex_shader_path = Configuration::GetInstance()->shader_path + std::string("LightShadingVex.glsl");

		std::string shader_source;
		if ((spotLights.size() + directionalLights.size() + pointLights.size()) > 0)
		{
			shader_source += std::string("#define ENABLE_LIGHTING\n");
		}
		if (spotLights.size() > 0)
		{
			shader_source += ( std::string("#define NR_SPOT_LIGHTS ") + SpotLightCount + '\n');
		}
		if (pointLights.size() > 0)
		{
			shader_source += (std::string("#define NR_POINT_LIGHTS ") + PointLightCount + '\n');
		}
		if (directionalLights.size() > 0)
		{
			shader_source += (std::string("#define NR_DIRECTIONAL_LIGHTS ") + DirectLightCount + '\n');
		}
		
		if (m_pMaterial->hasTextureMap(MATERIAL_DIFFUSE_MAP) || (0U != m_pMaterial->GetPropertyValue<glm::u32>(MATERIAL_DIFFUSE_MAP)))
		{
			shader_source += std::string("#define MATERIAL_DIFFUSE_MAP\n");
		}

		if (m_pMaterial->hasTextureMap(MATERIAL_SPECULAR_MAP) || (0U != m_pMaterial->GetPropertyValue<glm::u32>(MATERIAL_SPECULAR_MAP)))
		{
			shader_source += std::string("#define MATERIAL_SPECULAR_MAP\n");
		}

		auto fragment_shader_source = load(fragment_shader_path.c_str());
		fragment_shader_source = std::regex_replace(fragment_shader_source, std::regex("#version 330 core"), "");

		DynamicShader dynamic(name);
		dynamic.addFragmentSource({
			std::string("#version 330 core\n"),
			shader_source,
			fragment_shader_source
			}
		);

		dynamic.addVertexSource({load(vertex_shader_path.c_str())});

		pShaderProgram = dynamic.compile();
		Library::GetInstance()->add<ShaderProgram>(name, pShaderProgram);

		pShaderProgram->syncUniform(m_pMaterial);
	}

	return pShaderProgram;
}