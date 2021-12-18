#pragma once
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include <string>
#include "creator.h"

#include "GLTexture.h"

class GLFrameBufferObject: public creator<GLFrameBufferObject>
{
private:
	std::string	m_name;
	unsigned int m_framebuffer = 0U;
	unsigned int m_rbo = 0U;

	int m_width = 0;
	int m_height = 0;

	bool m_bInit = false;
	GLTexturePtr m_pTexture = nullptr;
protected:
	friend class creator<GLFrameBufferObject>;
	GLFrameBufferObject(std::string, int width, int height);
public:
	~GLFrameBufferObject();
	void init();
	std::string getName() const;
	GLuint getID() const;
	GLuint getTextureID() const;
	GLTexturePtr getTexture();
};

typedef std::shared_ptr<GLFrameBufferObject> GLFrameBufferObjectPtr;