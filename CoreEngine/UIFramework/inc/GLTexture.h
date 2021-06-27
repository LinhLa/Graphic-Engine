#pragma once
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <string>
#include <list>
#include "GLTypeDictionary.h"
#include "creator.h"

struct ScalarParam
{
	GL_TEXTURE_FUNC func;
	GLenum pname;
	GLenum param;
	ScalarParam::ScalarParam(GL_TEXTURE_FUNC f, GLenum name, GLenum param) :func(f), pname(name), param(param) {}
};

struct VectorParam
{
	GL_TEXTURE_FUNC func;
	GLenum pname;
	const void* params;
	VectorParam::VectorParam(GL_TEXTURE_FUNC f, GLenum name, const void* p) :func(f), pname(name), params(p) {}
};

class GLTexture: public creator<GLTexture>
{
private:
	std::string		m_name;
	std::string		m_path;
	GLuint 			m_textureID = 0;
	GLenum 			m_target = 0;

	bool m_isLoadToGPU = true;

	int m_width = 0;
	int m_height = 0;
	int m_Channels = 0;

	std::list<ScalarParam> m_scalarParamList;
	std::list<VectorParam> m_vectorParamList;

	void setTextureParam(GL_TEXTURE_FUNC func, GLenum pname, GLenum param);
	void setTextureParam(GL_TEXTURE_FUNC func, GLenum pname, const void* params);

protected:
	GLTexture(const std::string& name, const std::string& path, GLenum target);
public:
	virtual ~GLTexture();
	friend class creator<GLTexture>;

	void addTextureParam(GL_TEXTURE_FUNC func, GLenum pname, GLenum param);
	void addTextureParam(GL_TEXTURE_FUNC func, GLenum pname, const void* params);
	void setParam();

	void init();
	void load();

	int GetWidth() const;
	int GetHeight() const;

	void loadToGPU(bool flag = true);
	bool isLoadToGPU() const;

	void gen();
	void active(GLenum);
	void bind();
	void unbind();
	std::string getName() const;
	GLuint getID() const;
};

typedef std::shared_ptr<GLTexture> GLTexturePtr;