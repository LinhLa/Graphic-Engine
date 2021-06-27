#pragma once
#include "stdafx.h"
#include "Uniform.h"
#include "GLInfoLog.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "GLTexture.h"
#include "log.h"

std::shared_ptr<IUniform> create(std::string name, GLuint index, GLenum type, GLint size)
{
	IUniformPtr pUniform = nullptr;
	switch(type)
	{
		case GL_FLOAT:
		{
			pUniform = Uniform<glm::f32>::create(name, index, type, size);
			break;
		}
		case GL_FLOAT_VEC2:
		{
			pUniform = Uniform<glm::vec2>::create(name, index, type, size);
			break;
		}
		case GL_FLOAT_VEC3:
		{
			pUniform = Uniform<glm::vec3>::create(name, index, type, size);
			break;
		}
		case GL_FLOAT_VEC4:
		{
			pUniform = Uniform<glm::vec4>::create(name, index, type, size);
			break;
		}
		case GL_DOUBLE:
		{
			pUniform = Uniform<glm::f64>::create(name, index, type, size);
			break;
		}
		case GL_DOUBLE_VEC2:
		{
			pUniform = Uniform<glm::dvec2>::create(name, index, type, size);
			break;
		}
		case GL_DOUBLE_VEC3:
		{
			pUniform = Uniform<glm::dvec3>::create(name, index, type, size);
			break;
		}
		case GL_DOUBLE_VEC4:
		{
			pUniform = Uniform<glm::dvec4>::create(name, index, type, size);
			break;
		}
		case GL_INT:
		{
			pUniform = Uniform<glm::i32>::create(name, index, type, size);
			break;
		}
		case GL_UNSIGNED_INT:
		{
			pUniform = Uniform<glm::u32>::create(name, index, type, size);
			break;
		}
		case GL_SAMPLER_2D:
		{
			pUniform = Uniform<std::string>::create(name, index, type, size);
			break;
		}
		case GL_FLOAT_MAT4:
		{
			pUniform = Uniform<glm::mat4>::create(name, index, type, size);
			break;
		}
		default:
		{
			LOG_DEBUG("Not support type[%d]", type);
			break;
		}
	}
	return pUniform;
}

void IUniform::glUniform(GLint location, void* param)
{
	if (GL_SAMPLER_2D == this->getType())
	{
		GLint &rTextureLocation = *(GLint*)(param);
		glUniform1i(location, rTextureLocation++);
	}
	if (GL_FLOAT == this->getType())
	{
		glUniform1f(location, DynamicTypeCheck<glm::f32>()->get());
	}
	else if(GL_FLOAT_VEC3 == this->getType())
	{
		glm::vec3 v3 = DynamicTypeCheck<glm::vec3>()->get();
		glUniform3f(location, v3[0], v3[2], v3[2]);
	}
	else if (GL_FLOAT_VEC4 == this->getType())
	{
		glm::vec4 v4 = DynamicTypeCheck<glm::vec4>()->get();
		glUniform4f(location, v4[0], v4[2], v4[2], v4[3]);
	}
	else if (GL_DOUBLE_MAT4 == this->getType())
	{
		glm::mat4 mat4 = DynamicTypeCheck<glm::mat4>()->get();
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
	}
}