#include "stdafx.h"
#include "GLRenderManipulator.h"
#include "UIHelper.h"

#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include "log.h"
#include "WindowRender.h"
#include <stdexcept>

#include "Render.h"
#include "Scene.h"

#define IS_VALID_RECT(rect)		(0 != rect.x && 0 != rect.y && 0 != rect.h && 0 != rect.w)
#define INT2FLOAT(x) (static_cast<float>(x))
#define FLOAT2INT(x) (static_cast<int>(x))
#define SDL_INT2FLOAT(r) (SDL_FRect{INT2FLOAT(r.x), INT2FLOAT(r.y), INT2FLOAT(r.w), INT2FLOAT(r.h)})
#define SDL_FLOAT2INT(r) (SDL_Rect{FLOAT2INT(r.x), FLOAT2INT(r.y), FLOAT2INT(r.w), FLOAT2INT(r.h)})

glm::vec4 flipY(glm::vec4 box)
{
	auto screenHeight = static_cast<float>(WindowRender::GetInstance()->getHeight());
	auto windowHeight = box[3];
	box.y = screenHeight - windowHeight - box.y;
	return box;
}

static bool isValidRect(glm::vec4 box)
{
	return (0.0f != box[2] || 0.0f != box[3]);
}

static glm::vec4 toGLMVEC4(SDL_Rect rect)
{
	return glm::vec4(
		rect.x,
		rect.y,
		rect.w,
		rect.h
	);
}

/*==================================================================  GLRenderClipManipulator ==================================================================*/

GLRenderClipManipulator::GLRenderClipManipulator(const glm::vec4& target) : m_clip_target(target)
{
	glGetFloatv(GL_SCISSOR_BOX, glm::value_ptr(m_render_area));
	m_render_area = flipY(m_render_area);
	if (!isValidRect(m_render_area))
	{
		m_render_area = toGLMVEC4(UIHelper::GetWindowRect());
	}
}

GLRenderClipManipulator::~GLRenderClipManipulator()
{
	if (m_bSetClipTarget)
	{
		auto box = flipY(m_render_area);
		glScissor(box[0], box[1], box[2], box[3]);
	}
}

bool GLRenderClipManipulator::HasIntersection()
{
	glm::vec4 *AABBmin = &m_clip_target;
	glm::vec4 *AABBmax = &m_render_area;
	if (m_clip_target.x <= (m_render_area.x + m_render_area[2]))
	{
		AABBmin = &m_clip_target;
		AABBmax = &m_render_area;
	}
	else
	{
		AABBmin = &m_render_area;
		AABBmax = &m_clip_target;
	}
	return (AABBmin->x <= (AABBmax->x + (*AABBmax)[2]) && (AABBmin->x + (*AABBmin)[2] >= AABBmax->x) &&
         (AABBmin->y <= (AABBmax->y + (*AABBmax)[3]) && (AABBmin->y + (*AABBmin)[3]) >= AABBmax->y));
}

void GLRenderClipManipulator::SetRenderClipTarget()
{
	if (!m_bSetClipTarget)
	{
		auto box = flipY(m_clip_target);
		glScissor(box[0], box[1], box[2], box[3]);
	}
	m_bSetClipTarget = true;
}

std::stack<std::pair<bool, glm::ivec4>> GLRenderViewPortManipulator::m_stack;

GLRenderViewPortManipulator::GLRenderViewPortManipulator()
{}
GLRenderViewPortManipulator::~GLRenderViewPortManipulator()
{}

void GLRenderViewPortManipulator::pop()
{
	if (!m_stack.empty())
	{
		m_stack.pop();
		auto viewport = m_stack.top().second;
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	}
	else
	{
		LOG_DEBUG("ViewPort out of stack");
		_ASSERT(false);
	}
}

void GLRenderViewPortManipulator::push(glm::i32vec4 target_viewport, bool isFlipY)
{
	if (isFlipY)
	{
		target_viewport = flipY(target_viewport);
	}
	m_stack.push(std::make_pair(isFlipY, target_viewport));
	glViewport(target_viewport[0], target_viewport[1], target_viewport[2], target_viewport[3]);
}

glm::ivec4 GLRenderViewPortManipulator::top()
{
	auto viewport = m_stack.top().second;
	if (m_stack.top().first)
	{
		viewport = flipY(viewport);
	}
	return viewport;
}

void GLRenderViewPortManipulator::clear()
{
	m_stack = std::stack<std::pair<bool, glm::ivec4>>();
}




/*==================================================================  GLRenderFBOManipulator ==================================================================*/

std::stack<GLuint> GLRenderFBOManipulator::m_stack;

GLRenderFBOManipulator::GLRenderFBOManipulator()
{}
GLRenderFBOManipulator::~GLRenderFBOManipulator()
{}

void GLRenderFBOManipulator::pop()
{
	if (!m_stack.empty())
	{
		m_stack.pop();
		glBindFramebuffer(GL_FRAMEBUFFER, m_stack.top());
	}
	else
	{
		LOG_DEBUG("FBO out of stack");
		_ASSERT(false);
	}
}

void GLRenderFBOManipulator::push(GLuint target)
{
	m_stack.push(target);
	glBindFramebuffer(GL_FRAMEBUFFER, target);
}

GLuint GLRenderFBOManipulator::top()
{
	return m_stack.top();
}

void GLRenderFBOManipulator::clear()
{
	m_stack = std::stack<GLuint>();
}

/*==================================================================  GLRenderStackManipulator ==================================================================*/

std::stack<GLint> GLRenderStackManipulator::m_stack;

GLRenderStackManipulator::GLRenderStackManipulator()
{}
GLRenderStackManipulator::~GLRenderStackManipulator()
{}

void GLRenderStackManipulator::pop()
{
	if (!m_stack.empty())
	{
		m_stack.pop();
	}
	else
	{
		LOG_DEBUG("Render pass out of stack");
		_ASSERT(false);
	}
}

void GLRenderStackManipulator::push(GLint target)
{
	m_stack.push(target);
}

GLint GLRenderStackManipulator::top()
{
	return m_stack.top();
}

void GLRenderStackManipulator::clear()
{
	m_stack = std::stack<GLint>();
}

/*==================================================================  GLRenderContextManipulator ==================================================================*/
GLRenderContextManipulator::GLRenderContextManipulator() {}
GLRenderContextManipulator::~GLRenderContextManipulator() {}

void GLRenderContextManipulator::push(IGLRenderContextPtr context)
{
	if (context)
	{
		m_contexStack.push_back(context);
		m_pCurrentContext = context;
	}
}

void GLRenderContextManipulator::excute()
{
	if (m_pCurrentContext)
	{
		GLRenderStackManipulator::push(m_pCurrentContext->type());
		m_pCurrentContext->excute();
	}
}

void GLRenderContextManipulator::excuteAll()
{
	while (!m_contexStack.empty())
	{
		GLRenderStackManipulator::push(m_pCurrentContext->type());
		m_pCurrentContext->excute();	
		m_pCurrentContext->finish();
		
		pop();
		GLRenderStackManipulator::pop();
	}
}

void GLRenderContextManipulator::finish()
{
	m_pCurrentContext->finish();
	m_pCurrentContext = nullptr;
	m_contexStack.clear();
	GLRenderStackManipulator::pop();
}

bool GLRenderContextManipulator::empty()
{
	return m_contexStack.empty();
}

void GLRenderContextManipulator::pop()
{
	if (!m_contexStack.empty())
	{
		m_contexStack.pop_back();
		
		m_pCurrentContext = m_contexStack.empty()? nullptr : m_contexStack.back();
	}
	else
	{
		LOG_DEBUG("context out of stack");
		_ASSERT(false);
	}
}