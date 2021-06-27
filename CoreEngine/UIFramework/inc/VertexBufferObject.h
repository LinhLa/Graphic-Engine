#pragma once
#include <iostream>
#include <unordered_map>
#include <list>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include "creator.h"
#include "VertexData.h"

class IBufferObject
{
protected:
	VertexDataPtr m_pVertextData = nullptr;

	GLenum		m_BindingTarget = 0;
	uint32_t 	m_vboID = 0;
public:
	IBufferObject(GLenum target, VertexDataPtr pData);
	virtual ~IBufferObject();
	virtual void bindBuffer() = 0;
	void unbind();
	void genBuffer();

	uint32_t getID() const;
};
typedef std::shared_ptr<IBufferObject> IBufferObjectPtr;

class VertexBufferObject final: public IBufferObject, public creator<VertexBufferObject>
{
protected:
	VertexBufferObject(GLenum target, VertexDataPtr pData);
public:
	friend class creator<VertexBufferObject>;
	virtual ~VertexBufferObject();

	void bindBuffer() override;
	void debug();
};
 
typedef std::shared_ptr<VertexBufferObject> VertexBufferObjectPtr;

class ElementBufferObject final: public IBufferObject, public creator<ElementBufferObject>
{
protected:
	ElementBufferObject(GLenum target, VertexDataPtr pData);
public:
	friend class creator<ElementBufferObject>;
	virtual ~ElementBufferObject();

	void bindBuffer() override;
	void debug();
};

typedef std::shared_ptr<ElementBufferObject> ElementBufferObjectPtr;