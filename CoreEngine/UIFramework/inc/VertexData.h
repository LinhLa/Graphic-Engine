#pragma once
#include <iostream>
#include <vector>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "creator.h"
class VertexData final: public creator<VertexData>
{
private:
	GLenum				m_usage = 0;
	std::vector<float> 		m_vertices;
	std::vector<uint32_t> 	m_indices;
protected:
	VertexData(std::vector<float>&&, std::vector<uint32_t>&&, GLenum usage);
public:
	friend class creator<VertexData>;
	~VertexData();

	GLenum		usage() const;
		
	size_t sizeOfVertices() const;
	size_t sizeOfIndices() const;

	size_t countOfVertices() const;
	size_t countOfIndices() const;

	const void* dataOfVertices() const;
	const void* dataOfIndices() const;

	void debugVertices();
	void debugIndices();
};
 
typedef std::shared_ptr<VertexData> VertexDataPtr;