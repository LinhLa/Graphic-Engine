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

	void setUnifromMatrix(ShaderProgramPtr);
	void setUniform(ShaderProgramPtr pShaderProgram, MaterialPtr pMaterial);
	void activeTexture(std::vector<GLTexturePtr>& );
	void unbindTexture(std::vector<GLTexturePtr>& );

public:
	void DrawText2D(std::vector<CharacterPtr>,float opacity,glm::vec3 color);
	void DrawColor(glm::vec2 size, glm::vec4 color);
	void DrawImage(GLTexturePtr pTexture, float opacity, glm::vec4 color);
	void DrawGeometry(ShaderProgramPtr pShaderProgram, MaterialPtr pMaterial, ModelPtr pModel);

	glm::mat4 getModalMatrix() const;
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	void setModalMatrix(glm::mat4);
	void setViewMatrix(glm::mat4);
	void setProjectionMatrix(glm::mat4);
};