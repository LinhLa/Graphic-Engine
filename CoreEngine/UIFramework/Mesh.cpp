#pragma once
#include "stdafx.h"
#include "Mesh.h"
#include "GLInfoLog.h"
#include "log.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "stb_image.h"

Mesh::Mesh(const std::string& name, VertexDataPtr pVData):m_name(name)
{
	m_pVData = pVData;
	m_pVAO = VertexArrayObject::create();
	if (pVData->sizeOfVertices() > 0)
	{
		m_pVBO = VertexBufferObject::create(GL_ARRAY_BUFFER, pVData);
	}

	if (pVData->sizeOfIndices() > 0)
	{
		m_pEBO = ElementBufferObject::create(GL_ELEMENT_ARRAY_BUFFER, pVData);
	}
}

Mesh::~Mesh()
{
}

void Mesh::enableAttribute()
{
	for(auto &att: m_AttributeArray)
	{
		att.second->set();
		att.second->enable();
	}
}

void Mesh::addAttribute(VertexAttributePtr pAttribute)
{
	if (pAttribute)
	{
		m_AttributeArray[pAttribute->getName()] = pAttribute;
	}
}

void Mesh::init()
{
	m_pVAO->gen();
	if (m_pVBO)
	{
		m_pVBO->genBuffer();
	}

	if (m_pEBO)
	{
		m_pEBO->genBuffer();
	}

	m_pVAO->bind();
	if (m_pVBO)
	{
		m_pVBO->bindBuffer();
	}

	if (m_pEBO)
	{
		m_pEBO->bindBuffer();
	}

	enableAttribute();
	m_pVAO->unbind();
}

void Mesh::draw()
{
	m_pVAO->bind();
	if (m_pEBO)
	{
		glDrawElements(GL_TRIANGLES, m_pVData->countOfIndices(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_pVData->countOfVertices());
	}
	m_pVAO->unbind();
}

std::string Mesh::getName() const
{
	return m_name;
}

void Mesh::debug()
{
	LOG_DEBUG(".................................................................");
	LOG_DEBUG("name[%s]" , m_name.c_str());
	m_pVAO->debug();
	if (m_pVBO)
	{
		m_pVBO->debug();
	}

	if (m_pEBO)
	{
		m_pEBO->debug();
	}
	for(auto &att: m_AttributeArray)
	{
		att.second->debug();
	}
	LOG_DEBUG(".................................................................");
}