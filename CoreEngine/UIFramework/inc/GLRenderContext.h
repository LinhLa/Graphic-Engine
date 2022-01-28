#pragma once
#include "creator.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer3D.h"
#include "Library.h"
#include "GLFrameBufferObject.h"
#include "ShaderProgram.h"


#define GL_CLEAR_COLOR	glClearColor(0.2f, 0.3f, 0.3f, 0.0f)

extern const std::string SHADOWN_PROGRAM;
extern const std::string SHADOWN_MATERIAL;

extern const std::string INVERSION;
extern const std::string GRAYSCALE;


extern const std::string IDENTITY;
extern const std::string EDGE0;
extern const std::string EDGE1;
extern const std::string EDGE2;
extern const std::string SHARPEN;
extern const std::string BOX_BLUR;
extern const std::string GAUSSIAN_BLUR;
extern const std::string EMBOSS;


enum GL_RENDER_PASS : GLint
{
	DEFAULT_RENDER_PASS			= 1 << 0,
	MULTISAMPLE_RENDER_PASS		= 1 << 1,
	SHADOWN_MAP_RENDER_PASS		= 1 << 2,
	POST_PROCESSING_RENDER_PASS = 1 << 3,
};


enum GL_RENDER_EFFECT : GLint
{
	POST_PROCESSING_INVERSION,
	POST_PROCESSING_GRAYSCALE,
	POST_PROCESSING_IDENTITY,
	POST_PROCESSING_EDGE0,
	POST_PROCESSING_EDGE1,
	POST_PROCESSING_EDGE2,
	POST_PROCESSING_SHARPEN,
	POST_PROCESSING_BOX_BLUR,
	POST_PROCESSING_GAUSSIAN_BLUR,
	POST_PROCESSING_EMBOSS,
};

extern const std::map<std::string, GLint> gRenderPassMap;


class GLRenderContextManipulator;
class IGLRenderContext;
typedef std::shared_ptr<IGLRenderContext> IGLRenderContextPtr;

class IGLRenderContext
{
protected:
	friend class GLRenderContextManipulator;
	GLFrameBufferObjectPtr m_pFBO = nullptr;
	IGLRenderContext() {}

	virtual void excute() = 0;
	virtual void finish() = 0;
public:
	virtual ~IGLRenderContext() {}
	virtual GLint type() = 0;

	GLuint getBufferObject()
	{
		GLuint fbo = 0U;
		if (m_pFBO)
		{
			fbo = m_pFBO->getID();
		}
		return fbo;
	}

	static void blitFrameBuffer(GLuint from , GLuint to);
};

class GLRenderShadown : public creator<GLRenderShadown>, public IGLRenderContext
{
protected:
	GLRenderShadown(std::string name = "");
	GLFrameBufferObjectPtr m_pFBO = nullptr;
public:
	virtual ~GLRenderShadown();
	friend class creator<GLRenderShadown>;

	void excute() override;
	void finish() override;
	GLint type() override;
};

class GLRenderDefault : public creator<GLRenderDefault>, public IGLRenderContext
{
protected:
	GLRenderDefault(std::string name);
	GLFrameBufferObjectPtr m_pFBO = nullptr;
public:
	virtual ~GLRenderDefault();
	friend class creator<GLRenderDefault>;

	void excute() override;
	void finish() override;

	GLint type() override;
};

class GLRenderMultisample : public creator<GLRenderMultisample>, public IGLRenderContext
{
protected:
	GLRenderMultisample(std::string name);
	GLFrameBufferObjectPtr m_pFBO = nullptr;
public:
	virtual ~GLRenderMultisample();
	friend class creator<GLRenderMultisample>;

	void excute() override;
	void finish() override;

	GLint type() override;
};


class IKernel
{
public:
	IKernel() {}
	~IKernel() {}
	virtual GLint type() = 0;
	virtual std::string name() = 0;
	virtual std::string shader() = 0;
};

typedef std::shared_ptr<IKernel> IKernelPtr;

class InversionKernel final : public IKernel, public creator<InversionKernel>
{
	InversionKernel() {}
public:
	friend class creator<InversionKernel>;
	virtual ~InversionKernel() {}

	GLint type() override { return POST_PROCESSING_INVERSION; }
	std::string name() override { return (INVERSION); }
	std::string shader() override  { return "#define INVERSION\n";}
};

class GrayscaleKernel final : public IKernel, public creator<GrayscaleKernel>
{
	GrayscaleKernel() {}
public:
	friend class creator<GrayscaleKernel>;
	virtual ~GrayscaleKernel() {}

	GLint type() override { return POST_PROCESSING_GRAYSCALE; }
	std::string name() override { return (GRAYSCALE); }
	std::string shader() override { return "#define GRAYSCALE\n";}
};

class BlurKernel final : public IKernel, public creator<BlurKernel>
{
	BlurKernel() {}
public:
	friend class creator<BlurKernel>;
	virtual ~BlurKernel() {}

	GLint type() override { return POST_PROCESSING_GAUSSIAN_BLUR; }
	std::string name() override { return (GAUSSIAN_BLUR); }
	std::string shader() override { return ("#define GAUSSIAN_BLUR\n"); }
};

class GLRenderEffect : public creator<GLRenderEffect>, public IGLRenderContext
{
protected:
	std::list<IKernelPtr> m_kernelList;
	GLRenderEffect(std::string name, std::initializer_list<IKernelPtr> list = {});

	ShaderProgramPtr prepareShader();
public:
	virtual ~GLRenderEffect();
	friend class creator<GLRenderEffect>;

	void add(IKernelPtr pKernel);
	void excute() override;
	void finish() override;
	GLint type() override;
};