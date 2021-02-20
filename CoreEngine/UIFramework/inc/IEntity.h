#pragma once
#include <string>

class IEntity
{
private:
	//Animation   m_AnimationControl;
	int32_t     AnimationState = 0;

public:
	IEntity() = delete;
	IEntity(std::string);
	virtual ~IEntity();
};