#pragma once
#include <iostream>

#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include "creator.h"
class VertexArrayObject final: public creator<VertexArrayObject>
{
private:
	uint32_t 	m_vaoID  = 0;
protected:
	VertexArrayObject();
public:
	friend class creator<VertexArrayObject>;
	~VertexArrayObject();

	void gen();
	void bind();
	void unbind();
	
	uint32_t getID() const;
	std::string getName() const;

	void debug();
};

typedef std::shared_ptr<VertexArrayObject> VertexArrayObjectPtr;