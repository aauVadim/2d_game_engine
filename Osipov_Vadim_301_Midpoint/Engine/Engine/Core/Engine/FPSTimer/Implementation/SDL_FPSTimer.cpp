#include "SDL_FPSTimer.h"

#include "SDL.h"

SDLFPStimer::SDLFPStimer()
{
	m_startTime = SDL_GetTicks();
	m_timeSinceLastFrame = 0.0f;
	m_currentTime = 0;
	m_currentFrame = 0;
}

bool SDLFPStimer::Update()
{
	float currentFrame = static_cast<float>(SDL_GetTicks() - m_startTime) / 1000.0f;
	m_timeSinceLastFrame += currentFrame;
	m_startTime = SDL_GetTicks();

	if (m_timeSinceLastFrame > m_kUpdateRate)
	{
		m_timeSinceLastFrame = 0.0f;
		return true;
	}

	return true;
}

int SDLFPStimer::GetTickCount()
{
	return static_cast<int>(m_timeSinceLastFrame);
}
