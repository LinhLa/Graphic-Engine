#pragma once
#include <iostream>
#include <list>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "Shader.h"
#include "Uniform.h"
#include "Attribute.h"
#include "Material.h"
#include "creator.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

class ShaderProgram final: public creator<ShaderProgram>
{
private:
	std::string m_name;
	GLuint  	m_ProgramID  = 0;
	GLint		m_textureUnitCount = 0;

	std::vector<IUniformPtr>	m_UniformList;
	std::vector<AttributePtr>	m_AttributeList;

	ShaderPtr 	m_pFragment = nullptr;
	ShaderPtr   m_pVertex = nullptr;
	ShaderProgram(const std::string&);
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
	std::vector<AttributePtr>	querryAttribute();

	GLuint  getID() const;
	GLint   getTextureUnitCount() const;
	std::string getName() const;
	void syncUniform(std::shared_ptr<PropertyTable>);
	void setUniform(MaterialPtr);
	void setUnifromMatrix(glm::mat4, const char*);
};

typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;