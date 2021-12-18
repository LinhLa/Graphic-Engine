#include "stdafx.h"
#include <algorithm>

#include "Renderer3D.h"
#include "log.h"
#include "WindowRender.h"
#include "Library.h"
#include "log.h"
#include "Transformation2D.h"

#define MODEL_MATRIX		"model"
#define VIEW_MATRIX  		"view"
#define PROJECT_MATRIX  	"projection"
#define TEXT_SHADER			"Text"
#define TEXT_COLOR			"textColor"
#define TEXTURE_SHADER		"Texture"
#define COLOR_SHADER		"Color"

static unsigned int VAO = 0;
static unsigned int VBO = 0;
static unsigned int EBO = 0;
static void initTextureBuffer()
{
	static bool isInit = false;
	if (true == isInit)
	{
		return;
	}

	// set up indices for quad
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// set up buffer(s)) and configure vertex attributes
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 10 * 4, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	isInit = true;
}

static unsigned int framebuffer = 0U;
static unsigned int textureColorbuffer = 0U;
static 	unsigned int rbo = 0U;

static void initFrameBuffer()
{
	static bool isInit = false;
	if (true == isInit)
	{
		return;
	}
	float width = static_cast<float>(WindowRender::GetInstance()->getWidth());
	float height = static_cast<float>(WindowRender::GetInstance()->getHeight());

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// generate texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	// set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textureColorbuffer, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Set the list of draw buffers.
	//GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT1 };
	//glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	SDL_Log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	isInit = true;
}

Renderer3D::Renderer3D() :m_ModalMatrix(1.0f), m_ViewMatrix(1.0f), m_ProjectionMatrix(1.0f)
{
	initTextureBuffer();
	initFrameBuffer();
}

Renderer3D::~Renderer3D() {}

void Renderer3D::DrawColor(glm::vec2 coordinator, glm::vec2 size, glm::vec2 scale, float angle, glm::vec4 color)
{
	// Camera matrix
	m_ViewMatrix = glm::mat4(1.0f);

	// Model matrix : an identity matrix (model will be at the origin)
	m_ModalMatrix = glm::mat4(1.0f);

	// Projection matrix
	float width = static_cast<float>(WindowRender::GetInstance()->getWidth());
	float height = static_cast<float>(WindowRender::GetInstance()->getHeight());
	m_ProjectionMatrix = glm::ortho(0.0f, width, 0.0f, height);

	float w = size[0];
	float h = size[1];
	coordinator.y = height - coordinator.y - h;
	float vertices[] = {
		// positions								//color				
		coordinator.x + w, coordinator.y, 0.0f,		color.r, color.g, color.b, color.a,
		coordinator.x + w, coordinator.y + h, 0.0f,	color.r, color.g, color.b, color.a,
		coordinator.x, coordinator.y + h, 0.0f,		color.r, color.g, color.b, color.a,
		coordinator.x, coordinator.y, 0.0f,			color.r, color.g, color.b, color.a
	};

	auto pColorProgram = Library::GetInstance()->get<ShaderProgram>(COLOR_SHADER);
	pColorProgram->useProgram();
	setUnifromMatrix(pColorProgram);

	// update content of VBO memory
	glBindVertexArray(VAO);

	// update content of VBO memory
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// be sure to use glBufferSubData and not glBufferData
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// render quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer3D::DrawImage(GLTexturePtr pTexture, glm::vec2 coordinator, glm::vec2 scale, float angle, glm::vec2 origin, float opacity, glm::vec4 color)
{
	// View matrix
	m_ViewMatrix = glm::mat4(1.0f);

	// Projection matrix
	float width = static_cast<float>(WindowRender::GetInstance()->getWidth());
	float height = static_cast<float>(WindowRender::GetInstance()->getHeight());
	m_ProjectionMatrix = glm::ortho(0.0f, width, height, 0.0f);

	float w = static_cast<float>(pTexture->GetWidth());
	float h = static_cast<float>(pTexture->GetHeight());

	float quad[8] =
	{
		// positions
		coordinator.x + w, coordinator.y,		 // top right
		coordinator.x + w, coordinator.y + h,	 // bottom right
		coordinator.x, coordinator.y + h,		 // bottom left
		coordinator.x, coordinator.y,			 // top left 
	};

	if (abs(angle) >= 0.001f)
	{
		::rotate(quad, angle, origin.x, origin.y);
	}

	::scale(quad, scale.x, scale.y, origin.x, origin.y);

	float vertices[] = {
		// positions				//color									//opacity		// texture coords
		quad[0], quad[1], 0.0f,		color.r, color.g, color.b, color.a,		opacity,		1.0f, 1.0f, // top right
		quad[2], quad[3], 0.0f,		color.r, color.g, color.b, color.a,		opacity,		1.0f, 0.0f, // bottom right
		quad[4], quad[5], 0.0f,		color.r, color.g, color.b, color.a,		opacity,		0.0f, 0.0f, // bottom left
		quad[6], quad[7], 0.0f,		color.r, color.g, color.b, color.a,		opacity,		0.0f, 1.0f  // top left 
	};

	auto pTextureProgram = Library::GetInstance()->get<ShaderProgram>(TEXTURE_SHADER);
	pTextureProgram->useProgram();
	setUnifromMatrix(pTextureProgram);

	// active and bind texture
	glActiveTexture(GL_TEXTURE0);
	pTexture->bind();

	// update content of VBO memory
	glBindVertexArray(VAO);

	// update content of VBO memory
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// be sure to use glBufferSubData and not glBufferData
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// opacity attribute
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// texture coord attribute
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// render quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	pTexture->unbind();
}

void Renderer3D::DrawText2D(
	std::vector<CharacterPtr> characterList,
	glm::vec2 coordinator,
	glm::vec2 scale,
	float angle,
	glm::vec2 origin,
	float opacity,
	glm::vec3 color)
{
	float width = static_cast<float>(WindowRender::GetInstance()->getWidth());
	float height = static_cast<float>(WindowRender::GetInstance()->getHeight());

	// View matrix
	m_ViewMatrix = glm::mat4(1.0f);

	// Model matrix : an identity matrix (model will be at the origin)
	m_ModalMatrix = glm::mat4(1.0f);

	// Projection matrix
	m_ProjectionMatrix = glm::ortho(0.0f, width, height, 0.0f);

	// Calculate final matrix
	glm::mat4 m_TransformMatrix = m_ProjectionMatrix * m_ViewMatrix * m_ModalMatrix;

	// activate corresponding render state	
	auto pTextProgram = Library::GetInstance()->get<ShaderProgram>(TEXT_SHADER);
	pTextProgram->useProgram();
	glUniformMatrix4fv(glGetUniformLocation(pTextProgram->getID(), PROJECT_MATRIX), 1, GL_FALSE, glm::value_ptr(m_TransformMatrix));
	glUniform3f(glGetUniformLocation(pTextProgram->getID(), TEXT_COLOR), color.r, color.g, color.b);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	for (auto& ch : characterList)
	{
		float xpos = coordinator.x + ch->mBearing.x;
		float ypos = coordinator.y - (ch->mSize.y - ch->mBearing.y);

		float w = ch->mSize.x;
		float h = ch->mSize.y;

		float quad[8] =
		{
			// positions
			xpos + w, ypos,     // top right
			xpos + w, ypos + h, // bottom right
			xpos,     ypos + h, // bottom left
			xpos,     ypos,     // top left 
		};

		if (abs(angle) >= 0.001f)
		{
			::rotate(quad, angle, origin.x, origin.y);
		}

		::scale(quad, scale.x, scale.y, origin.x, origin.y);

		// update VBO for each character
		float vertices[4][4] = {
			//pos				//uv - flip top bottom for top left origin
			{quad[0], quad[1],  1.0f, 0.0f},
			{quad[2], quad[3],  1.0f, 1.0f},
			{quad[4], quad[5],  0.0f, 1.0f},
			{quad[6], quad[7],  0.0f, 0.0f}
		};

		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch->mTextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// render quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		coordinator.x += (ch->mAdvance >> 6) * scale.x; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer3D::DrawGeometry(ShaderProgramPtr pShaderProgram, MaterialPtr pMaterial /*std::vector<GLTexturePtr>& list*/, ModelPtr pModel)
{
	glEnable(GL_DEPTH_TEST);
	pShaderProgram->useProgram();
	setUniform(pShaderProgram, pMaterial);
	setUnifromMatrix(pShaderProgram);
	//activeTexture(list);
	pModel->Draw(pShaderProgram);

	glDisable(GL_DEPTH_TEST);
}
glm::mat4 Renderer3D::getModalMatrix() const { return m_ModalMatrix; }
glm::mat4 Renderer3D::getViewMatrix() const { return m_ViewMatrix; }
glm::mat4 Renderer3D::getProjectionMatrix() const { return m_ProjectionMatrix; }

void Renderer3D::setModalMatrix(glm::mat4 matrix)
{
	m_ModalMatrix = matrix;
}

void Renderer3D::setViewMatrix(glm::mat4 matrix)
{
	m_ViewMatrix = matrix;
}

void Renderer3D::setProjectionMatrix(glm::mat4 matrix)
{
	m_ProjectionMatrix = matrix;
}

void Renderer3D::setUnifromMatrix(ShaderProgramPtr pShaderProgram)
{
	GLint uniformLocation = glGetUniformLocation(pShaderProgram->getID(), PROJECT_MATRIX);
	if (-1 != uniformLocation)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));
	}
	else
	{
		LOG_DEBUG("Shader program[%s] No uniform location for [%s]", pShaderProgram->getName().c_str(), PROJECT_MATRIX);
	}

	uniformLocation = glGetUniformLocation(pShaderProgram->getID(), VIEW_MATRIX);
	if (-1 != uniformLocation)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m_ViewMatrix));
	}
	else
	{
		LOG_DEBUG("Shader program[%s] No uniform location for [%s]", pShaderProgram->getName().c_str(), VIEW_MATRIX);
	}

	uniformLocation = glGetUniformLocation(pShaderProgram->getID(), MODEL_MATRIX);
	if (-1 != uniformLocation)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(m_ModalMatrix));
	}
	else
	{
		LOG_DEBUG("Shader program[%s] No uniform location for [%s]", pShaderProgram->getName().c_str(), MODEL_MATRIX);
	}
}

void Renderer3D::activeTexture(std::vector<GLTexturePtr>& list)
{
	glEnable(GL_TEXTURE_2D);
	GLint max_texture_units;
	GLint count = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);

	// bind textures on corresponding texture units
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->active();
		count++;
		if (max_texture_units <= count)
		{
			LOG_DEBUG("Over max texture units: Max[%d] Current[%d]", max_texture_units, list.size());
			break;
		}
	}
}

void Renderer3D::unbindTexture(std::vector<GLTexturePtr>& list)
{
	for (auto& pTexture : list)
	{
		pTexture->unbind();
	}
}

void Renderer3D::setUniform(ShaderProgramPtr pShaderProgram, MaterialPtr pMaterial)
{
	auto m_UniformList = pShaderProgram->querryUniform();
	GLint textureLocation = 0;
	GLenum location = GL_TEXTURE0;

	glEnable(GL_TEXTURE_2D);
	for (auto& pUniform : m_UniformList)
	{
		int location = glGetUniformLocation(pShaderProgram->getID(), pUniform->getName().c_str());
		if (-1 != location)
		{
			if (GL_SAMPLER_2D == pUniform->getType())
			{
				if (pMaterial->hasTextureMap(pUniform->getName()))
				{
					auto pTexture = pMaterial->GetTexture(pUniform->getName());
					pUniform->glUniform(location, (void*)&textureLocation);
					pTexture->setLocation(location);
					pTexture->active();
					textureLocation++;
				}
			}
			else
			{
				pUniform->glUniform(location, nullptr);
			}
		}
		else
		{
			LOG_DEBUG("Couldn't get uniform location for:%s", pUniform->getName().c_str());
		}
	}

	GLint max_texture_units;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);

	if (max_texture_units <= textureLocation)
	{
		LOG_DEBUG("Over max texture units: Max[%d] Current[%d]", max_texture_units, textureLocation);
	}
}