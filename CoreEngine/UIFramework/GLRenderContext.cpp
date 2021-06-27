#include "stdafx.h"
#include "GLRenderContext.h"
#include <exception>
#include <algorithm>
#include "log.h"
#include "Library.h"
#include "Configuration.h"
#include "Renderer3D.h"

#define INT2FLOAT(x) (static_cast<float>(x))
#define SDL_INT2FLOAT(r) (SDL_FRect{INT2FLOAT(r.x), INT2FLOAT(r.y), INT2FLOAT(r.w), INT2FLOAT(r.h)})

GLRender2DContext::GLRender2DContext(
	ShaderProgramPtr pShaderProgram,
	MeshPtr	pMesh,
	std::vector<GLTexturePtr> textureList,
	SDL_Rect& srcrect,
	SDL_Rect& dstrect,
	double angle,
	SDL_Point& center,
	SDL_RendererFlip flip)
{
	m_pShaderProgram = pShaderProgram;
	m_pMesh = pMesh;
	m_textureList = textureList;
	m_srcrect = SDL_INT2FLOAT(srcrect);
	m_dstrect = SDL_INT2FLOAT(dstrect);
	m_angle = angle;
	m_center = { INT2FLOAT(center.x), INT2FLOAT(center.y) };
	m_flip = flip;
}
GLRender2DContext::~GLRender2DContext() {}

void GLRender2DContext::calculateMatrix()
{
	glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 ModalMatrix = glm::mat4(1.0f);
	glm::mat4 ViewMatrix = glm::mat4(1.0f);

	glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 scale = glm::vec3(0.3f);
	
	ModalMatrix = glm::translate(ModalMatrix, translate);
	ModalMatrix = glm::rotate(ModalMatrix, glm::radians(0.0f), rotate);
	ModalMatrix = glm::scale(ModalMatrix, scale);

	Renderer3D::GetInstance()->setModalMatrix(ModalMatrix);
	Renderer3D::GetInstance()->setViewMatrix(ViewMatrix);
	Renderer3D::GetInstance()->setProjectionMatrix(ProjectionMatrix);
}

void GLRender2DContext::calculateMatrixBackground()
{
	glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 ModalMatrix = glm::mat4(1.0f);
	glm::mat4 ViewMatrix = glm::mat4(1.0f);

	glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.001f);
	glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 scale = glm::vec3(0.3f);

	ModalMatrix = glm::translate(ModalMatrix, translate);
	ModalMatrix = glm::rotate(ModalMatrix, glm::radians(0.0f), rotate);
	ModalMatrix = glm::scale(ModalMatrix, scale);

	Renderer3D::GetInstance()->setModalMatrix(ModalMatrix);
	Renderer3D::GetInstance()->setViewMatrix(ViewMatrix);
	Renderer3D::GetInstance()->setProjectionMatrix(ProjectionMatrix);
}

void GLRender2DContext::excute()
{
	calculateMatrixBackground();
	Renderer3D::GetInstance()->DrawBackgroundColor(m_pMesh);

	calculateMatrix();
	Renderer3D::GetInstance()->DrawImage(m_textureList, m_pShaderProgram, m_pMesh);
};

GLRender3DContext::GLRender3DContext(
	const glm::vec3& scale,
	const glm::vec3& translate,
	const glm::vec3& rotate,
	float angle, 
	std::vector<GLTexturePtr> list,
	ShaderProgramPtr pProgram,
	MeshPtr pMesh):
m_scale(scale), m_translate(translate),m_rotate(rotate),m_angle(angle)
{
	m_TextureList = list;
	m_pShaderProgram = pProgram;
	m_pMesh = pMesh;
	if (!m_pMesh)
	{
		throw std::logic_error("invalid argument");
	}
}

GLRender3DContext::~GLRender3DContext() {}

void GLRender3DContext::excute()
{
	Renderer3D::GetInstance()->DrawGeometry(m_pShaderProgram, m_TextureList, m_pMesh);
}

void GLRender3DContext::calculateMatrix()
{
	glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 ModalMatrix = glm::mat4(1.0f);
	glm::mat4 ViewMatrix = glm::mat4(1.0f);

	glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 scale = glm::vec3(0.3f);

	ModalMatrix = glm::translate(ModalMatrix, translate);
	ModalMatrix = glm::rotate(ModalMatrix, glm::radians(0.0f), rotate);
	ModalMatrix = glm::scale(ModalMatrix, scale);

	Renderer3D::GetInstance()->setModalMatrix(ModalMatrix);

	// note that we're translating the scene in the reverse direction of where we want to move
	ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
	Renderer3D::GetInstance()->setViewMatrix(ViewMatrix);

	float width = static_cast<float>(Configuration::GetInstance()->width);
	float height = static_cast<float>(Configuration::GetInstance()->height);

	//ProjectionMatrix = glm::ortho(0.0f, width, 0.0f, height, 0.0f, 100.0f);
	//ProjectionMatrix = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
	Renderer3D::GetInstance()->setProjectionMatrix(ProjectionMatrix);
}