#pragma once
#include <iostream>
#include <unordered_map>

#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "creator.h"

#include "VertexData.h"
#include "VertexAttribute.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

class Mesh final: public creator<Mesh>
{
private:
	friend class Renderer3D;
	std::string m_name;
	VertexDataPtr			m_pVData = nullptr;
	VertexBufferObjectPtr 	m_pVBO = nullptr;
	ElementBufferObjectPtr 	m_pEBO = nullptr;
	VertexArrayObjectPtr	m_pVAO = nullptr;
	std::unordered_map<std::string, VertexAttributePtr>	m_AttributeArray;
	
	void enableAttribute();
protected:
	Mesh(const std::string& name, VertexDataPtr);
public:
	friend class creator<Mesh>;
	~Mesh();

	void init();
	void addAttribute(VertexAttributePtr);
	
	std::string getName() const;
	void draw();
	void debug();
};

typedef std::shared_ptr<Mesh> MeshPtr;