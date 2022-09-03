#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include "creator.h"
#include "IPropertyMethod.h"
#include "IProperty.h"


enum CAMERA_TYPE_ENUM : int
{
	ORTHOGRAPHIC = 0,
	PERSPECTIVE,
};

extern const std::map<std::string, CAMERA_TYPE_ENUM> cameraMap;

class CameraProperty final: public IPropertyMethod ,public creator<CameraProperty>
{
protected:
	PropertyTable* m_pPropertyTable = nullptr;
	CameraProperty(PropertyTable*);
public:
	~CameraProperty();
	friend class creator<CameraProperty>;

	CameraProperty& operator=(const CameraProperty& rhs) = delete;
	void init() override;

	glm::vec3 GetCamPos() const;
	void SetCamPos(glm::vec3 value);

	glm::vec3 GetCamFront() const;
	void SetCamFront(glm::vec3 value);

	glm::vec3 GetCamTarget() const;
	void SetCamTarget(glm::vec3 value);

	glm::vec3 CameraProperty::GetCamUp() const;
	void CameraProperty::SetCamUp(glm::vec3 value);

	float GetCamPitch() const;
	void SetCamPitch(float value);

	float GetCamYaw() const;
	void SetCamYaw(float value);

	float GetCamSpeed() const;
	void SetCamSpeed(float value);

	float GetFOV() const;
	void SetFOV(float value);

	float CameraProperty::GetNearPlane() const;
	void CameraProperty::SetNearPlane(float value);

	float CameraProperty::GetFarPlane() const;
	void CameraProperty::SetFarPlane(float value);

	int GetCameraType() const;
	void SetCameraType(int value);
};

typedef std::shared_ptr<CameraProperty> CameraPropertyPtr;