#pragma once
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <string>
#include "creator.h"
class Attribute final: public creator<Attribute>
{
private:
	std::string		m_name;
	GLuint 			m_index = 0;
	GLenum 			m_type = 0;
	GLint 			m_size = 0;

protected:
	Attribute(std::string, GLuint, GLenum, GLint);
public:
	~Attribute();
	friend class creator<Attribute>;

	std::string getName() const;
	void debug();
};

typedef std::shared_ptr<Attribute> AttributePtr;