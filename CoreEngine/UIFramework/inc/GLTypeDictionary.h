#pragma once
#include <unordered_map>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

enum GL_TEXTURE_FUNC : int
{
	TEXT_PARAM_F = 0,
	TEXT_PARAM_I,
	TEXT_PARAM_FV,
	TEXT_PARAM_IV,
	TEXT_PARAM_IIV,
	TEXT_PARAM_IUIV,
	TEXTURE_PARAM_F,
	TEXTURE_PARAM_I,
	TEXTURE_PARAM_FV,
	TEXTURE_PARAM_IV,
	TEXTURE_PARAM_IIV,
	TEXTURE_PARAM_IUIV,
};

extern const std::unordered_map<GLenum, std::string> GLSL_TYPE_DICTIONARY;
extern const std::unordered_map<GLenum, std::string> OPENGL_TYPE_DICTIONARY;
extern const std::unordered_map<std::string, GLenum> ENUM_TYPE_FROMSTRING;
extern const std::unordered_map<std::string, size_t> SIZE_TYPE_FROMSTRING;
extern const std::unordered_map<std::string, GLenum> GL_TEXTURE_TARGET_MAP;
extern const std::unordered_map<std::string, GLenum> GL_TEXTURE_PNAME_MAP;
extern const std::unordered_map<std::string, GLenum> GL_TEXTURE_PVALUE_MAP;
extern const std::unordered_map<std::string, GL_TEXTURE_FUNC> GL_TEXTURE_FUNC_MAP; 
extern const std::unordered_map<std::string, GLenum> GL_BUFFER_BINDING_TARGET_MAP;
extern const std::unordered_map<std::string, GLenum> GL_BUFFER_USAGE_MAP;
extern const std::unordered_map<std::string, GLenum> GL_PRIMITIVE_MAP;