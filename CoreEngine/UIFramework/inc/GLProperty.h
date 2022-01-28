#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include "creator.h"
#include "IPropertyMethod.h"
#include "IProperty.h"
#include "ShaderProgram.h"
#include "GLTexture.h"


enum CAMERA_TYPE_ENUM : int
{
	ORTHOGRAPHIC = 0,
	PERSPECTIVE,
};

extern const std::map<std::string, CAMERA_TYPE_ENUM> cameraMap;

class GLProperty final: public IPropertyMethod ,public creator<GLProperty>
{
protected:
	PropertyTable* m_pPropertyTable = nullptr;
	GLProperty(PropertyTable*);
public:
	~GLProperty();
	friend class creator<GLProperty>;

	GLProperty& operator=(const GLProperty& rhs) = delete;
	void init() override;

	float 	GetBlendIndensity() const;
	void 	SetBlendIndensity(float value);

	glm::vec3 GetCamPos() const;
	void SetCamPos(glm::vec3 value);

	glm::vec3 GetCamFront() const;
	void SetCamFront(glm::vec3 value);

	glm::vec3 GetCamTarget() const;
	void SetCamTarget(glm::vec3 value);

	glm::vec3 GLProperty::GetCamUp() const;
	void GLProperty::SetCamUp(glm::vec3 value);

	float GetCamPitch() const;
	void SetCamPitch(float value);

	float GetCamYaw() const;
	void SetCamYaw(float value);

	float GetCamSpeed() const;
	void SetCamSpeed(float value);

	float GetFOV() const;
	void SetFOV(float value);

	float GLProperty::GetNearPlane() const;
	void GLProperty::SetNearPlane(float value);

	float GLProperty::GetFarPlane() const;
	void GLProperty::SetFarPlane(float value);

	int GetCameraType() const;
	void SetCameraType(int value);

	glm::i32vec4 GetViewPort() const;
	void SetViewPort(glm::i32vec4 value);
};

typedef std::shared_ptr<GLProperty> GLPropertyPtr;