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
#include "Mesh.h"

#define TRANSFORM_MATRIX	"transform"
#define MODEL_MATRIX		"model"
#define VIEW_MATRIX  		"view"
#define PROJECT_MATRIX  	"projection"


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
	void setUniform(ShaderProgramPtr , std::vector<GLTexturePtr>& );
	void bindTextureUnit(std::vector<GLTexturePtr>& );
	void unbindTexture(std::vector<GLTexturePtr>& );

public:
	void DrawBackgroundColor(MeshPtr pTextureGeometry);
	void DrawImage(std::vector<GLTexturePtr> textureList, ShaderProgramPtr pTextureProgram, MeshPtr pTextureGeometry);
	void DrawGeometry(ShaderProgramPtr pShaderProgram, std::vector<GLTexturePtr>& list, MeshPtr pMesh);

	glm::mat4 getModalMatrix() const;
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	void setModalMatrix(glm::mat4);
	void setViewMatrix(glm::mat4);
	void setProjectionMatrix(glm::mat4);
};