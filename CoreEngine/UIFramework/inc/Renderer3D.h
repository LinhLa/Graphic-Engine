#pragma once
#include <vector>

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include "SingletonHolder.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "ShaderProgram.h"
#include "GLTexture.h"
#include "GLFrameBufferObject.h"

#include "Model.h"
#include "Mesh.h"
#include "GLFont.h"

class Renderer3D final : public SingletonHolder<Renderer3D>
{
private:
	friend class SingletonHolder<Renderer3D>;
	Renderer3D();
	~Renderer3D();
	
	glm::mat4  		m_ModalMatrix;
	glm::mat4  		m_ViewMatrix;
	glm::mat4  		m_ProjectionMatrix;

	SDL_FRect		m_clip_rect;
	glm::vec2		m_scale;

	void setUnifromMatrix(ShaderProgramPtr);
	void setUniform(ShaderProgramPtr , std::vector<GLTexturePtr>& );
	void bindTextureUnit(std::vector<GLTexturePtr>& );
	void unbindTexture(std::vector<GLTexturePtr>& );

public:
	void DrawText(
		std::vector<CharacterPtr>,
		glm::vec2 coordinator,
		glm::vec2 scale,
		float angle,
		glm::vec2 origin,
		float opacity,
		glm::vec3 color);

	void DrawColor(glm::vec2 coordinator, glm::vec2 size, glm::vec2 scale, float angle, glm::vec4 color);
	void DrawImage(GLTexturePtr pTexture, glm::vec2 coordinator, glm::vec2 scale, float angle, glm::vec2 origin, float opacity, glm::vec4 color);
	void DrawGeometry(ShaderProgramPtr pShaderProgram, std::vector<GLTexturePtr>& list, ModelPtr pModel);

	glm::mat4 getModalMatrix() const;
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	void setModalMatrix(glm::mat4);
	void setViewMatrix(glm::mat4);
	void setProjectionMatrix(glm::mat4);

	void setRenderClipTarget(SDL_FRect);
	SDL_FRect getRenderClipTarget() const;

	void setRenderScale(float X, float Y);
	void getRenderScale(float &X, float &Y) const;
};