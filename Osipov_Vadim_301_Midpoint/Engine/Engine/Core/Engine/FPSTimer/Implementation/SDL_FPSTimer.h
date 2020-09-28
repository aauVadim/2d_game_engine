#pragma once

#include "../FPSTimer.h"

class SDLFPStimer : public FPSTimer
{
	int m_kUpdateRate = 33; 

	unsigned int m_startTime;
	unsigned int m_currentFrame; 
	int m_currentTime; 

	float m_timeSinceLastFrame; 


public:
	SDLFPStimer();
	virtual bool Update() override;
	virtual int GetTickCount() override;
};