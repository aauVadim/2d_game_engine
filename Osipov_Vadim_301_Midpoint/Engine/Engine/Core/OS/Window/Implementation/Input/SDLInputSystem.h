#pragma once

#include <SDL.h>

class SDLInputSystem
{
	enum class MouseState
	{
		kPressed, 
		kReleased,
	};
	MouseState m_mouseState;
public:
	SDLInputSystem();
	void OnButtonDown(SDL_Event& inputEvent, const float& offsetX = 0.0f, const float& offsetY = 0.0f);
	void OnButtonUp(SDL_Event& inputEvent, const float& offsetX = 0.0f, const float& offsetY = 0.0f);
private:
	void ProcessMouseInput(SDL_Event& inputEvent, const float& offsetX = 0.0f, const float& offsetY = 0.0f);
	void ProcessKeyboardInput(SDL_Event& inputEvent);
};
