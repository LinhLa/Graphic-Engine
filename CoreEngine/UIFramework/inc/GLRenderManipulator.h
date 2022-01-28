#pragma once
#include <stack>
#include <SDL.h>
#include "GLRenderContext.h"
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec4 flipY(glm::vec4 box);

/**
 * @brief      This class describes a render clip manipulator.
 */
class GLRenderClipManipulator
{
private:
	glm::vec4 m_clip_target;
	glm::vec4 m_render_area;
	bool	 m_bSetClipTarget = false;
public:
	GLRenderClipManipulator(const glm::vec4& target);
	~GLRenderClipManipulator();

	bool HasIntersection();
	bool GetIntersection(glm::vec4&);

	void SetRenderClipTarget();
};


/**
 * @brief      This class describes a render view port manipulator.
 */
class GLRenderViewPortManipulator
{
private:
	static std::stack<glm::i32vec4> m_stack;
protected:
	GLRenderViewPortManipulator();
	~GLRenderViewPortManipulator();
public:
	static void pop();
	static void push(glm::i32vec4);
	static void clear();
	static glm::i32vec4 top();
};

/**
 * @brief      This class describes a render fbo manipulator.
 */
class GLRenderFBOManipulator
{
private:
	static std::stack<GLuint> m_stack;
protected:
	GLRenderFBOManipulator();
	~GLRenderFBOManipulator();
public:
	static void pop();
	static void push(GLuint);
	static void clear();
	static GLuint top();
};


/**
 * @brief      This class describes a render stack manipulator.
 */
class GLRenderStackManipulator
{
private:
	static std::stack<GLint> m_stack;
protected:
	GLRenderStackManipulator();
	~GLRenderStackManipulator();
public:
	static void pop();
	static void push(GLint);
	static void clear();
	static GLint top();
};

/**
 * @brief      This class describes a render context manipulator.
 */
class GLRenderContextManipulator
{
private:
	std::list<IGLRenderContextPtr>	m_contexStack;
	IGLRenderContextPtr				m_pCurrentContext;

	void pop();
public:
	GLRenderContextManipulator();
	~GLRenderContextManipulator();

	void push(IGLRenderContextPtr);

	void excute();
	void excuteAll();
	void finish();
	bool empty();
};