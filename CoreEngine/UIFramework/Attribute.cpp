#pragma once
#include "stdafx.h"
#include "Attribute.h"
#include "GLInfoLog.h"
#include "log.h"
#include "GLTypeDictionary.h"
Attribute::Attribute(std::string name, GLuint index, GLenum type, GLint size):m_name(name), m_index(index), m_type(type), m_size(size)
{}

Attribute::~Attribute()
{}

std::string Attribute::getName() const
{
	return m_name;
}

void Attribute::debug()
{
	LOG_DEBUG("..................................");
	LOG_DEBUG("name[%s]" , m_name.c_str());
	LOG_DEBUG("index[%d]" , m_index);
	LOG_DEBUG("type[%s]" , GLSL_TYPE_DICTIONARY.at(m_type).c_str());
	LOG_DEBUG("size[%d]" , m_size);
}