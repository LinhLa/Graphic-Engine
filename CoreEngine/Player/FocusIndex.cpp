#include "stdafx.h"
#include "IFocusComponent.h"

int32_t IFocusComponent::s_count = 0;

IFocusComponent::IFocusComponent()
{
	m_iFocusIndex = s_count++;
}

IFocusComponent::~IFocusComponent(){}

int32_t IFocusComponent::getFocusIndex() const
{
	return m_iFocusIndex;
}