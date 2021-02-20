#include "stdafx.h"
#include "FpsTracking.h"

FpsTracking::FpsTracking(){}
FpsTracking::~FpsTracking(){}

void FpsTracking::start(){m_fpsTimer.start();}
float FpsTracking::getFps(){ return m_avgFPS;}

void FpsTracking::countFrames()
{
	++m_countedFrames;
	m_avgFPS = float(m_countedFrames) / m_fpsTimer.getSeconds();
}
