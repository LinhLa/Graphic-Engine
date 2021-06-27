#include "stdafx.h"
#include <algorithm>

#include "Renderer3D.h"
#include "log.h"
#include "Configuration.h"
#include "Library.h"

Renderer3D::Renderer3D():m_ModalMatrix(1.0f), m_ViewMatrix(1.0f),m_ProjectionMatrix(1.0f) {}
Renderer3D::~Renderer3D() {}

void Renderer3D::DrawImage(std::vector<GLTexturePtr> textureList, ShaderProgramPtr pTextureProgram, MeshPtr pTextureGeometry)
{
	bindTextureUnit(textureList);
	pTextureProgram->useProgram();
	setUniform(pTextureProgram, textureList);
	setUnifromMatrix(pTextureProgram);
	pTextureGeometry->draw();	
	unbindTexture(textureList);
}

void Renderer3D::DrawBackgroundColor(MeshPtr pTextureGeometry)
{
	auto pTextureProgram = Library::GetInstance()->get<ShaderProgram>("Color");
	pTextureProgram->useProgram();
	int location = glGetUniformLocation(pTextureProgram->getID(), "backgroundColor");
	glUniform4f(location, 0.6f, 0.7f, 0.3f, 1.0f);
	setUnifromMatrix(pTextureProgram);
	pTextureGeometry->draw();
}

void Renderer3D::DrawGeometry(ShaderProgramPtr pShaderProgram, std::vector<GLTexturePtr>& list, MeshPtr pMesh)
{
	bindTextureUnit(list);
	pShaderProgram->useProgram();
	setUniform(pShaderProgram, list);
	setUnifromMatrix(pShaderProgram);
	pMesh->draw();
	unbindTexture(list);
}

glm::mat4 Renderer3D::getModalMatrix() const { return m_ModalMatrix; }
glm::mat4 Renderer3D::getViewMatrix() const { return m_ViewMatrix; }
glm::mat4 Renderer3D::getProjectionMatrix() const { return m_ProjectionMatrix; }

void Renderer3D::setModalMatrix(glm::mat4 matrix)
{
	m_ModalMatrix = matrix;
}

void Renderer3D::setViewMatrix(glm::mat4 matrix)
{
	m_ViewMatrix = matrix;
}

void Renderer3D::setProjectionMatrix(glm::mat4 matrix)
{
	m_ProjectionMatrix = matrix;
}

void Renderer3D::setUnifromMatrix(ShaderProgramPtr pShaderProgram)
{
	GLint uniformLocation = glGetUniformLocation(pShaderProgram->getID(), PROJECT_MATRIX);
	if (-1 != uniformLocation)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));
	}
	else
	{
		LOG_DEBUG("Couldn't get uniform location for:%s", PROJECT_MATRIX);
	}

	uniformLocation = glGetUniformLocation(pShaderProgram->getID(), VIEW_MATRIX);
	if (-1 != uniformLocation)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m_ViewMatrix));
	}
	else
	{
		LOG_DEBUG("Couldn't get uniform location for:%s", VIEW_MATRIX);
	}

	uniformLocation = glGetUniformLocation(pShaderProgram->getID(), MODEL_MATRIX);
	if (-1 != uniformLocation)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m_ModalMatrix));
	}
	else
	{
		LOG_DEBUG("Couldn't get uniform location for:%s", MODEL_MATRIX);
	}
}


void Renderer3D::bindTextureUnit(std::vector<GLTexturePtr>& list)
{
	glEnable(GL_TEXTURE_2D);
	GLint max_texture_units;
	GLenum location = GL_TEXTURE0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);

	// bind textures on corresponding texture units
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->active(location++);
		if ((GL_TEXTURE0 + max_texture_units) <= static_cast<GLint>(location))
		{
			LOG_DEBUG("Over max texture units: Max[%d] Current[%d]", max_texture_units, list.size());
			break;
		}
	}
}

void Renderer3D::unbindTexture(std::vector<GLTexturePtr>& list)
{
	for (auto& pTexture : list)
	{
		pTexture->unbind();
	}
	glDisable(GL_TEXTURE_2D);
}

void Renderer3D::setUniform(ShaderProgramPtr pShaderProgram, std::vector<GLTexturePtr>& list)
{
	auto m_UniformList = pShaderProgram->querryUniform();
	GLint textureLocation = 0;
	for (auto& pUniform : m_UniformList)
	{
		int location = glGetUniformLocation(pShaderProgram->getID(), pUniform->getName().c_str());
		if (-1 != location)
		{
			if (GL_SAMPLER_2D == pUniform->getType())
			{
				if (1 == pShaderProgram->getTextureUnitCount())
				{
					//default texture, no need to set uniform
					continue;
				}
				for (auto itr = list.begin(); itr != list.end(); ++itr)
				{
					if (0 == pUniform->get<std::string>().compare((*itr)->getName()))
					{
						// calculating the index
						textureLocation = itr - list.begin();
						break;
					}
				}
			}
			pUniform->glUniform(location, (void*)&textureLocation);
		}
		else
		{
			LOG_DEBUG("Couldn't get uniform location for:%s", pUniform->getName().c_str());
		}
	}
}
