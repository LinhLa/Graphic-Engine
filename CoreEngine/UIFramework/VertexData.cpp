#pragma once
#include "stdafx.h"
#include <sstream>
#include "VertexData.h"
#include "GLInfoLog.h"
#include "log.h"

VertexData::VertexData(std::vector<float>&& vertices, std::vector<uint32_t>&& indices, GLenum usage):
m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_usage(usage)
{}

VertexData::~VertexData()
{}

GLenum VertexData::usage() const
{
	return m_usage;
}

size_t VertexData::sizeOfVertices() const
{
	return m_vertices.size() * sizeof(float);
}

size_t VertexData::sizeOfIndices() const
{
	return m_indices.size() * sizeof(uint32_t);
}

size_t VertexData::countOfVertices() const
{
	return m_vertices.size();
}

size_t VertexData::countOfIndices() const
{
	return m_indices.size();
}

const void* VertexData::dataOfVertices() const
{
	return m_vertices.data();
}

const void* VertexData::dataOfIndices() const
{
	return m_indices.data();
}

void VertexData::debugVertices()
{
	std::stringstream stream;
	stream << "Vertex:";
	if (!m_vertices.empty())
	{
		for (auto& v : m_vertices)
		{
			stream << v << " ";
			stream.seekg(0, std::ios::end);
			if (stream.tellg() >= 64)
			{
				LOG_DEBUG("%s", stream.str().c_str());
				stream.str("");
			}
		}
	}
}

void VertexData::debugIndices()
{
	std::stringstream stream;
	stream << "Indices:";
	if (!m_indices.empty())
	{
		for (auto& i : m_indices)
		{
			stream << i << " ";
			stream.seekg(0, std::ios::end);
			if (stream.tellg() >= 64)
			{
				LOG_DEBUG("%s", stream.str().c_str());
				stream.str("");
			}
		}

	}
}