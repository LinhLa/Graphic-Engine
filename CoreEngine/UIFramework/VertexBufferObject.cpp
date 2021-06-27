#pragma once
#include "stdafx.h"
#include "VertexBufferObject.h"
#include "GLInfoLog.h"
#include "log.h"
#include <stdexcept>
IBufferObject::IBufferObject(GLenum target, VertexDataPtr pData):m_BindingTarget(target)
{
	if (!pData)
	{
		throw std::logic_error("invalid argument");
	}
	m_pVertextData = pData;
}

IBufferObject::~IBufferObject()
{
	glDeleteBuffers(1, &m_vboID);
}

void IBufferObject::unbind()
{
	glBindBuffer(m_BindingTarget, 0);
}

void IBufferObject::genBuffer()
{
	glGenBuffers(1, &m_vboID);
}

uint32_t IBufferObject::getID() const
{
	return m_vboID;
}

VertexBufferObject::VertexBufferObject(GLenum target, VertexDataPtr pData): IBufferObject(target, pData)
{}

VertexBufferObject::~VertexBufferObject() {}

void VertexBufferObject::bindBuffer()
{
	glBindBuffer(m_BindingTarget, m_vboID);
	glBufferData(m_BindingTarget, m_pVertextData->sizeOfVertices(), m_pVertextData->dataOfVertices(), m_pVertextData->usage());
}

void VertexBufferObject::debug()
{
	LOG_DEBUG("VBO_ID[%d]" , m_vboID);
	m_pVertextData->debugVertices();
}

ElementBufferObject::ElementBufferObject(GLenum target, VertexDataPtr pData): IBufferObject(target, pData)
{}

ElementBufferObject::~ElementBufferObject() {}

void ElementBufferObject::bindBuffer()
{
	glBindBuffer(m_BindingTarget, m_vboID);
    glBufferData(m_BindingTarget, m_pVertextData->sizeOfIndices(), m_pVertextData->dataOfIndices() , m_pVertextData->usage());
}

void ElementBufferObject::debug()
{
	LOG_DEBUG("EBO_ID[%d]", m_vboID);
	m_pVertextData->debugIndices();
}