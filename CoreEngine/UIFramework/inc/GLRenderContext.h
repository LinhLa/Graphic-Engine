#pragma once
#include "Model.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "GLTexture.h"
#include "Camera.h"
#include "creator.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLRender2DContext : public creator<GLRender2DContext>
{
protected:
	GLRender2DContext(
		GLTexturePtr pTexture,
		glm::vec2 coordinator,
		glm::vec2 scale,
		glm::vec4 color,
		float angle,
		float opcity,
		glm::vec2 center,
		SDL_RendererFlip flip);

	void calculateMatrix();
public:
	virtual ~GLRender2DContext();
	friend class creator<GLRender2DContext>;

	void excute();
private:
	GLTexturePtr	m_pTexture = nullptr;
	glm::vec2		m_coordinator;
	glm::vec2		m_scale;
	glm::vec4		m_color;
	float			m_angle;
	float			m_opcity;
	glm::vec2		m_center;
	SDL_RendererFlip m_flip = SDL_FLIP_NONE;
};
typedef std::shared_ptr<GLRender2DContext> GLRender2DContextPtr;

class GLRender3DContext : public creator<GLRender3DContext>
{
protected:
	GLRender3DContext(
		glm::vec3  scale,
		glm::vec3  translate,
		glm::vec3  rotate,
		float	angle,
		ShaderProgramPtr,
		MaterialPtr,
		ModelPtr,
		CameraPtr);
public:
	virtual ~GLRender3DContext();
	friend class creator<GLRender3DContext>;

	void excute();
private:
	glm::vec3  		m_scale;
	glm::vec3  		m_translate;
	glm::vec3  		m_rotate;
	float     		m_angle;

	std::vector<GLTexturePtr> m_TextureList;

	ShaderProgramPtr m_pShaderProgram = nullptr;
	ModelPtr		m_pModel = nullptr;
	MaterialPtr		m_pMaterial = nullptr;
	CameraPtr		m_pCamera = nullptr;
	void calculateMatrix();
	void processTextureMap();
};
typedef std::shared_ptr<GLRender3DContext> GLRender3DContextPtr;

