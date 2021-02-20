#pragma once
#include <SDL.h>
/**
 * @brief      This class describes a frame rate capping.
 */
class FrameRateCapping
{
private:
	//<Capping frame rate
	uint32_t m_delayTime = 0;
	uint32_t m_FrameStart = 0;
	uint32_t m_FrameTime = 0;
public:
	FrameRateCapping();
	~FrameRateCapping();

	void Capping();
	void FrameStart();
};
