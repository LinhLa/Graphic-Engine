#pragma once
#include <list>

#include "Mesh.h"
#include "ShaderProgram.h"
#include "GLTexture.h"
#include "creator.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GLRender2DContext : public creator<GLRender2DContext>
{
protected:
	GLRender2DContext(
		ShaderProgramPtr pShaderProgram,
		MeshPtr	pMesh,
		std::vector<GLTexturePtr> textureList,
		SDL_Rect& srcrect,
		SDL_Rect& dstrect,
		double angle,
		SDL_Point& center,
		SDL_RendererFlip flip);
public:
	virtual ~GLRender2DContext();
	friend class creator<GLRender2DContext>;

	void excute();
private:
	ShaderProgramPtr	m_pShaderProgram = nullptr;
	MeshPtr				m_pMesh = nullptr;
	std::vector<GLTexturePtr> m_textureList;
	SDL_FRect		m_srcrect;
	SDL_FRect		m_dstrect;
	double			m_angle;
	SDL_FPoint		m_center;
	SDL_RendererFlip m_flip = SDL_FLIP_NONE;

	void calculateMatrix();
	void calculateMatrixBackground();
};
typedef std::shared_ptr<GLRender2DContext> GLRender2DContextPtr;

class GLRender3DContext : public creator<GLRender3DContext>
{
protected:
	GLRender3DContext(
		const glm::vec3&  scale,
		const glm::vec3&  translate,
		const glm::vec3&  rotate,
		float	angle,
		std::vector<GLTexturePtr> list,
		ShaderProgramPtr pProgram,
		MeshPtr);
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
	MeshPtr			m_pMesh = nullptr;

	void calculateMatrix();
};
typedef std::shared_ptr<GLRender3DContext> GLRender3DContextPtr;

