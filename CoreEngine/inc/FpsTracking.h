#pragma once
#include <Timer.h>
/**
 * @brief      This class describes a fps tracking.
 */
class FpsTracking
{
private:
	Timer 		m_fpsTimer;
	float 		m_avgFPS = 0.0F;
	uint32_t 	m_countedFrames = 0;
public:
	FpsTracking();
	~FpsTracking();

	void 		start();
	float 		getFps();
	void 		countFrames();
};
