#pragma once
#include "stdafx.h"
#include "VertexAttribute.h"
#include "GLInfoLog.h"
#include "log.h"
#include "GLTypeDictionary.h"
VertexAttribute::VertexAttribute(const std::string& name, int32_t index, int32_t size, GLenum type, bool normalized, size_t stride, const void* offset):
m_name(name), m_index(index), m_size(size), m_type(type), m_normalized(normalized), m_stride(stride), m_offset(offset)
{}

VertexAttribute::~VertexAttribute()
{}

void VertexAttribute::debug()
{
	LOG_DEBUG("..................................");
	LOG_DEBUG("Attribute[%s]" , m_name.c_str());
	LOG_DEBUG("index[%d]" , m_index);
	LOG_DEBUG("type[%s]" , OPENGL_TYPE_DICTIONARY.at(m_type).c_str());
	LOG_DEBUG("size[%d]" , m_size);
	LOG_DEBUG("stride[%d]" , m_stride);
	LOG_DEBUG("offset[%d]" , m_offset);
}

void VertexAttribute::set()
{
	glVertexAttribPointer(m_index, m_size, m_type, m_normalized, m_stride, m_offset);
}

void VertexAttribute::enable()
{
	glEnableVertexAttribArray(m_index);
}

void VertexAttribute::disable()
{
	glDisableVertexAttribArray(m_index);
}

std::string VertexAttribute::getName() const
{
	return m_name;
}