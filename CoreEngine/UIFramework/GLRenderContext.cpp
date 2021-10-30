#include "stdafx.h"
#include "GLRenderContext.h"
#include <exception>
#include <algorithm>
#include "log.h"
#include "Library.h"
#include "Configuration.h"
#include "Renderer3D.h"

GLRender2DContext::GLRender2DContext(
	GLTexturePtr pTexture,
	glm::vec2 coordinator,
	glm::vec2 scale,
	glm::vec4 color,
	float angle,
	float opcity,
	glm::vec2 center,
	SDL_RendererFlip flip)
{
	m_pTexture = pTexture;
	m_coordinator = coordinator,
	m_scale = scale;
	m_color = color;
	m_angle = angle;
	m_opcity = opcity;
	m_center = center;
	m_flip = flip;
}
GLRender2DContext::~GLRender2DContext() {}

void GLRender2DContext::excute()
{
	//Render geometry
	Renderer3D::GetInstance()->DrawImage(
		m_pTexture,
		m_coordinator,
		m_scale,
		m_angle,
		m_center,
		m_opcity,
		m_color);
};

GLRender3DContext::GLRender3DContext(
	glm::vec3  scale,
	glm::vec3  translate,
	glm::vec3  rotate,
	float	angle,
	ShaderProgramPtr pProgram,
	MaterialPtr pMaterial,
	ModelPtr pModel,
	CameraPtr pCamera):
	m_scale(scale), m_translate(translate), m_rotate(rotate), m_angle(angle)
{
	m_pShaderProgram = pProgram;
	m_pModel = pModel;
	m_pMaterial = pMaterial;
	m_pCamera = pCamera;
	if (!m_pModel || !m_pMaterial || !m_pShaderProgram || !pCamera)
	{
		throw std::logic_error("invalid argument");
	}
}

GLRender3DContext::~GLRender3DContext() {}

void GLRender3DContext::excute()
{
	processTextureMap();

	//Set matrix for Coordinator System
	calculateMatrix();

	//Render Geometry
	Renderer3D::GetInstance()->DrawGeometry(m_pShaderProgram, m_TextureList, m_pModel);
}

void GLRender3DContext::processTextureMap()
{
	auto m_UniformList = m_pShaderProgram->querryUniform();
	std::vector<GLTexturePtr> list;

	//filter texture list by uniform
	for (auto& pUniform : m_UniformList)
	{
		if (GL_SAMPLER_2D == pUniform->getType())
		{
			//for (auto itr = m_TextureList.begin(); itr != m_TextureList.end(); ++itr)
			{
				if (m_pMaterial->hasTextureMap(pUniform->getName()))
				//if (0 == pUniform->get<std::string>().compare((*itr)->getName()))
				{
					auto pTexture = m_pMaterial->GetTexture(pUniform->getName());
					list.push_back(pTexture);

					//reset location
					pTexture->setLocation(0U);
					break;
				}
			}
		}
	}

	m_TextureList = std::move(list);
	if ((1 == m_pShaderProgram->getTextureUnitCount()) && (1 == m_TextureList.size()))
	{
		(*m_TextureList.begin())->setLocation(GL_TEXTURE0);
	}
}

void GLRender3DContext::calculateMatrix()
{
	glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 ModalMatrix = glm::mat4(1.0f);
	glm::mat4 ViewMatrix = glm::mat4(1.0f);

	ModalMatrix = glm::translate(ModalMatrix, m_translate);
	ModalMatrix = glm::rotate(ModalMatrix, glm::radians(m_angle), m_rotate);
	ModalMatrix = glm::scale(ModalMatrix, m_scale);

	Renderer3D::GetInstance()->setModalMatrix(ModalMatrix);

	ViewMatrix = m_pCamera->View();
	Renderer3D::GetInstance()->setViewMatrix(ViewMatrix);

	float width = static_cast<float>(Configuration::GetInstance()->width);
	float height = static_cast<float>(Configuration::GetInstance()->height);

	ProjectionMatrix = glm::perspective(glm::radians(m_pCamera->Zoom()), width / height, m_pCamera->NearPlane(), m_pCamera->FarPlane());
	Renderer3D::GetInstance()->setProjectionMatrix(ProjectionMatrix);
}