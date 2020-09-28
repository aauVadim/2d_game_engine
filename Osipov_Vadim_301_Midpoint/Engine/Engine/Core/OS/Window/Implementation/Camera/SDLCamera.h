#pragma once

#include <SDL.h>
#include "../../../../Engine/Event System/EventListener.h"

class SDLCamera : public EventListener
{
	SDL_Point m_mousePos;
	SDL_Point m_screenCenter;
	SDL_Rect* m_pCameraRect;
	SDL_Rect* m_pScreenRect;
	SDL_Rect* m_pMarqueeRect; 
	bool m_canMoveCamera;
	float m_panSpeed;

	float m_offsetX, m_offsetY;

	bool m_shouldMarqueeSelect; 
	bool m_isMarqueeActive; 
	int m_marqueeStartX, m_marqueeStartY;
public:
	SDLCamera();
	~SDLCamera();
	void UpdateWindowState(SDL_Event& event);
	void UpdateCameraPosition();

	float GetCameraOffsetX() const { return m_offsetX; }
	float GetCameraOffsetY() const { return m_offsetY; }

	virtual void OnEvent(const Event* pEvent) override;

	SDL_Rect* GetMarqueeRect();

private:
	void MoveCamera(const SDL_Point& mousePos);
};