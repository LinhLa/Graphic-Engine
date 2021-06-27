#pragma once
#include "stdafx.h"
#include "VertexArrayObject.h"
#include "GLInfoLog.h"
#include "log.h"

VertexArrayObject::VertexArrayObject()
{}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &m_vaoID);
}

void VertexArrayObject::gen()
{
	glGenVertexArrays(1, &m_vaoID);
}

void VertexArrayObject::bind()
{
	glBindVertexArray(m_vaoID);
}

void VertexArrayObject::unbind()
{
	glBindVertexArray(0);
}

uint32_t VertexArrayObject::getID() const
{
	return m_vaoID;
}

void VertexArrayObject::debug()
{
	LOG_DEBUG("VAO_ID[%d]" , m_vaoID);
}