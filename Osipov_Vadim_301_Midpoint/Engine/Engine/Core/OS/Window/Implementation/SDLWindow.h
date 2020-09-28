#pragma once
#include <SDL.h>
#include "../Virtuals/IWindow.h"

struct SDL_Window;

class SDLInputSystem;
class SDLCamera;

class SDLWindow : public IWindow
{
	SDL_Window* m_pWindow;
	
	SDLInputSystem* m_pInput; 
	SDLCamera* m_pCamera;


public:
	SDLWindow();
	~SDLWindow();

	virtual bool Initialize() override;
	virtual IWindow* GetWindow() override;
	virtual void Update() override;

	SDLCamera* GetWindowCamera() const { return m_pCamera; }
};