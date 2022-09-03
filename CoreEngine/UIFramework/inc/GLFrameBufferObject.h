#pragma once
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include <string>
#include "creator.h"

#include "GLTexture.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLFrameBufferObject: public creator<GLFrameBufferObject>
{
private:
	std::string	m_name;
	GLuint m_framebuffer = 0U;
	GLuint m_rbo = 0U;

	glm::i32vec2 m_resolution;

	GLTexturePtr m_pTexture = nullptr;
protected:
	friend class creator<GLFrameBufferObject>;
	GLFrameBufferObject(std::string, int width, int height);
	GLFrameBufferObject(std::string, glm::i32vec4);

	bool isInit();
public:
	~GLFrameBufferObject();
	void initTexture2D(GLenum  attachment = GL_COLOR_ATTACHMENT0);
	void initTexture2DMS(GLenum  attachment = GL_COLOR_ATTACHMENT0, GLsizei sample = 4);
	void initDepthMap(GLenum attachment = GL_DEPTH_COMPONENT16);

	std::string getName() const;
	GLuint getID() const;

	GLuint getTextureID() const;
	GLTexturePtr getTexture();
};

typedef std::shared_ptr<GLFrameBufferObject> GLFrameBufferObjectPtr;