#include "stdafx.h"

#include <exception>
#include <algorithm>
#include "log.h"

#include "Configuration.h"
#include "GLRenderContext.h"
#include "GLRenderManipulator.h"

#include "DynamicShader.h"

const std::string SHADOWN_PROGRAM = "DepthShader";

const std::string INVERSION = "Inversion";
const std::string GRAYSCALE = "Grayscale";


const std::string IDENTITY = "identity";
const std::string EDGE0 = "edge0";
const std::string EDGE1 = "edge1";
const std::string EDGE2 = "edge2";
const std::string SHARPEN = "sharpen";
const std::string BOX_BLUR = "box blur";
const std::string GAUSSIAN_BLUR = "Gaussian Blur";
const std::string EMBOSS = "emboss";

const std::map<std::string, GLint> gRenderPassMap =
{
	{"default", DEFAULT_RENDER_PASS},
	{"Multisamle", MULTISAMPLE_RENDER_PASS},
	{"Shadown mapping", SHADOWN_MAP_RENDER_PASS},
	{"post processing", POST_PROCESSING_RENDER_PASS}
};

void IGLRenderContext::blitFrameBuffer(GLuint from_fbo, GLuint to_fbo)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, from_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, to_fbo);
	auto viewport = GLRenderViewPortManipulator::top();
	glBlitFramebuffer(
		viewport[0], viewport[1], viewport[2], viewport[3],
		viewport[0], viewport[1], viewport[2], viewport[3],
		GL_COLOR_BUFFER_BIT,
		GL_NEAREST);
}

GLRenderDepthMap::GLRenderDepthMap(std::string name)
{
	name = "GLRenderDepthMap_" + name;
	m_pFBO = Library::GetInstance()->get<GLFrameBufferObject>(name);
	if (!m_pFBO)
	{
		auto resolution = Configuration::GetInstance()->shadown_resolution;
		m_pFBO = GLFrameBufferObject::create(name, resolution, resolution);
		Library::GetInstance()->add<GLFrameBufferObject>(name, m_pFBO);
	}
}

GLRenderDepthMap::~GLRenderDepthMap() {}

GLint GLRenderDepthMap::type()
{
	return SHADOWN_MAP_RENDER_PASS;
}

void GLRenderDepthMap::excute()
{
	m_pFBO->initDepthMap();
	//<set view port to shadow resolution
	auto resolution = Configuration::GetInstance()->shadown_resolution;
	GLRenderViewPortManipulator::push((glm::i32vec4(0, 0, resolution, resolution)));

	GLRenderFBOManipulator::push(m_pFBO->getID());
	glClear(GL_DEPTH_BUFFER_BIT);
}

void GLRenderDepthMap::finish()
{
	//pop viewport shadow
	GLRenderViewPortManipulator::pop();

	// pop fbo
	GLRenderFBOManipulator::pop();

	// clear color + depth buffer before rendering scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLRenderDefault::GLRenderDefault(std::string name)
{
	name = "GLRenderDefault_" + name;
	m_pFBO = Library::GetInstance()->get<GLFrameBufferObject>(name);
	if (!m_pFBO)
	{
		m_pFBO = GLFrameBufferObject::create(name, GLRenderViewPortManipulator::top());
		Library::GetInstance()->add<GLFrameBufferObject>(name, m_pFBO);
	}
}

GLRenderDefault::~GLRenderDefault() {}

void GLRenderDefault::excute()
{
	m_pFBO->initTexture2D();

	// blit to intermediate buffer
	blitFrameBuffer(GLRenderFBOManipulator::top(), m_pFBO->getID());
	
	GLRenderFBOManipulator::push(m_pFBO->getID());
}

void GLRenderDefault::finish()
{
	// pop fbo
	GLRenderFBOManipulator::pop();

	// blit to intermediate buffer
	blitFrameBuffer(m_pFBO->getID(), GLRenderFBOManipulator::top());
}

GLint GLRenderDefault::type()
{
	return DEFAULT_RENDER_PASS;
}

GLRenderMultisample::GLRenderMultisample(std::string name)
{
	name = "GLRenderMultisample_" + name;
	m_pFBO = Library::GetInstance()->get<GLFrameBufferObject>(name);
	if (!m_pFBO)
	{
		m_pFBO = GLFrameBufferObject::create(name, GLRenderViewPortManipulator::top());
		Library::GetInstance()->add<GLFrameBufferObject>(name, m_pFBO);
	}
}

GLRenderMultisample::~GLRenderMultisample() {}

void GLRenderMultisample::excute()
{
	m_pFBO->initTexture2DMS();

	// blit to intermediate buffer
	blitFrameBuffer(GLRenderFBOManipulator::top(), m_pFBO->getID());

	GLRenderFBOManipulator::push(m_pFBO->getID());
}

void GLRenderMultisample::finish()
{
	// pop fbo
	GLRenderFBOManipulator::pop();

	// blit from intermediate buffer
	blitFrameBuffer(m_pFBO->getID(), GLRenderFBOManipulator::top());
}

GLint GLRenderMultisample::type()
{
	return MULTISAMPLE_RENDER_PASS;
}


GLRenderEffect::GLRenderEffect(std::string name, std::initializer_list<IKernelPtr> list)
{
	name = "GLRenderEffect_" + name;
	m_pFBO = Library::GetInstance()->get<GLFrameBufferObject>(name);
	if (!m_pFBO)
	{
		m_pFBO = GLFrameBufferObject::create(name, GLRenderViewPortManipulator::top());
		Library::GetInstance()->add<GLFrameBufferObject>(name, m_pFBO);
	}
	m_kernelList.insert(m_kernelList.end(), list);
}

GLRenderEffect::~GLRenderEffect() {}

ShaderProgramPtr GLRenderEffect::prepareShader()
{
	std::string name = POSTPROCESSING_SHADER;
	auto viewpot = GLRenderViewPortManipulator::top();
	auto width = std::to_string(viewpot[2]);
	auto height = std::to_string(viewpot[3]);
	auto iResolution = "vec2 iResolution = vec2(" + width + "," + height + ");\n";
	name += (width + "." + height);
	std::string shader_source = std::string("#version 330 core\n");
	for (auto& kernel : m_kernelList)
	{
		name += ('_' + kernel->name());
		shader_source += kernel->shader();
	}
	auto pShaderProgram = Library::GetInstance()->get<ShaderProgram>(name);
	if (!pShaderProgram)
	{
		//<load vertex + fragment shader
		std::string fragment_shader_path = Configuration::GetInstance()->shader_path + std::string("PostProcessingFrag.glsl");
		std::string vertex_shader_path = Configuration::GetInstance()->shader_path + std::string("PostProcessingVex.glsl");

		DynamicShader dynamic(name);
		dynamic.addFragmentSource({
			shader_source,
			iResolution,
			load(fragment_shader_path.c_str())
			}
		);

		dynamic.addVertexSource({ load(vertex_shader_path.c_str()) });

		pShaderProgram = dynamic.compile();
		Library::GetInstance()->add<ShaderProgram>(name, pShaderProgram);
	}
	return pShaderProgram;
}

void GLRenderEffect::add(IKernelPtr pKernel)
{
	if (pKernel)
		m_kernelList.push_back(pKernel);
}

void GLRenderEffect::excute()
{
	m_pFBO->initTexture2D();

	// blit from intermediate buffer
	blitFrameBuffer(GLRenderFBOManipulator::top(), m_pFBO->getID());

	// push fbo
	GLRenderFBOManipulator::push(m_pFBO->getID());

	auto pShader = prepareShader();
	Renderer3D::GetInstance()->setModalMatrix(glm::mat4(1.0f));
	Renderer3D::GetInstance()->DrawImage(m_pFBO->getTexture(), 1.0f, glm::vec4(1.0f), pShader);
}

void GLRenderEffect::finish()
{
	// pop fbo
	GLRenderFBOManipulator::pop();

	// blit to intermediate buffer
	blitFrameBuffer(m_pFBO->getID(), GLRenderFBOManipulator::top());
}

GLint GLRenderEffect::type() { return POST_PROCESSING_RENDER_PASS; }
