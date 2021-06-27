#pragma once
#include <iostream>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "creator.h"
class VertexAttribute final: public creator<VertexAttribute>
{
private:
	std::string m_name;

	int32_t 	m_index = 0;
	int32_t		m_size = 0;
	GLenum		m_type = 0;
	bool		m_normalized = false;
	size_t		m_stride;
	const void* m_offset = nullptr;
protected:
	VertexAttribute(const std::string& name, int32_t index, int32_t size, GLenum type, bool normalized, size_t stride, const void* offset);
public:
	friend class creator<VertexAttribute>;
	~VertexAttribute();

	void debug();
	void set();
	void enable();
	void disable();

	std::string getName() const;
};
 
typedef std::shared_ptr<VertexAttribute> VertexAttributePtr;