#pragma once
#include <iostream>
#include <list>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "Shader.h"
#include "Uniform.h"
#include "Attribute.h"
#include "creator.h"
#include "UIObject.h"
class ShaderProgram final: public creator<ShaderProgram>
{
private:
	std::string m_name;
	GLuint  	m_ProgramID  = 0;
	GLint		m_textureUnitCount = 0;
	ShaderProgram(const std::string&);
	std::vector<IUniformPtr>	m_UniformList;
	std::vector<AttributePtr>	m_AttributeList;
public:
	friend class creator<ShaderProgram>;
	~ShaderProgram();

	void attachShader(ShaderPtr);
	void linkProgram();
	void useProgram();

	void read();
	void write();
	void debug();

	std::vector<IUniformPtr> 	querryUniform();
	std::vector<AttributePtr> querryAttribute();

	GLuint  getID() const;
	GLint   getTextureUnitCount() const;
	std::string getName() const;
	void syncUniformToUIObject(UIObjectPtr pObject);
	void setUniformFromUIObject(UIObjectPtr pObject);
};

typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;