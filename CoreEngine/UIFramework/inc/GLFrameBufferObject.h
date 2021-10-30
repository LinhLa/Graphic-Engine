#pragma once
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include <string>
#include <list>

#include "GLTypeDictionary.h"
#include "GLTexture.h"

#include "creator.h"

class GLFrameBufferObject: public creator<GLFrameBufferObject>
{
private:
	std::string	m_name;
	GLuint		m_fbo = 0;
	GLTexturePtr m_pTexture = nullptr;

protected:
	friend class creator<GLFrameBufferObject>;
	GLFrameBufferObject(std::string, GLTexturePtr);
public:
	~GLFrameBufferObject();
	void init();
	std::string getName() const;
	GLuint getID() const;
};

typedef std::shared_ptr<GLFrameBufferObject> GLFrameBufferObjectPtr;