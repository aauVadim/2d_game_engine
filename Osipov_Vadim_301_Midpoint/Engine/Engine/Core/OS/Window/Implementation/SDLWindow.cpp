#include <SDL_rect.h>
//Temp Config 
#include "../../../Config.h"
#include "../../../UtilMacros.h"
#include "SDLWindow.h"
//Input
#include "Input/SDLInputSystem.h"
#include "Camera/SDLCamera.h"

#include "../../../ApplicationStates.h"
#include "../../../Engine/Engine.h"
//Event system
#include "../../../Engine/Event System/EventSystem.h"
#include "../../../Engine/Event System/EventTypes.h"
#include "../../../Engine/Event System/ConcreteEvents/EventOnGameWindowClosed.h"

#include "../../Console/Logger.h"

SDLWindow::SDLWindow()
	: m_pWindow(nullptr)
	, m_pInput(new SDLInputSystem())
	, m_pCamera(new SDLCamera())
{

}

SDLWindow::~SDLWindow()
{
	_SAFE_DELETE_(m_pInput);
	//For some reason I have to delete Camera this way, macro wasn't working. 
	delete m_pCamera;

	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

bool SDLWindow::Initialize()
{
	SDL_Init(SDL_INIT_VIDEO);
	m_pWindow = SDL_CreateWindow("Vadim Osipov - Game Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		g_kWindowWidth, g_kWindowHeight,
		SDL_WINDOW_SHOWN);

	return m_pWindow != nullptr;
}

IWindow* SDLWindow::GetWindow()
{
	return (IWindow*)m_pWindow;
}

void SDLWindow::Update()
{
	SDL_Event event;
	SDL_PumpEvents();
	//do update 
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_WINDOWEVENT)
		{
			//Handling the close window event 
			if (event.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				EventSystem::AddEventForExecution(new EventOnGameWindowClosed());
			}
			//Updating camera state. 
			m_pCamera->UpdateWindowState(event);

		}
		//Processing mouse and keyboard events
		if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYDOWN)
			m_pInput->OnButtonDown(event, m_pCamera->GetCameraOffsetX(), m_pCamera->GetCameraOffsetY());
		else if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_KEYUP)
			m_pInput->OnButtonUp(event, m_pCamera->GetCameraOffsetX(), m_pCamera->GetCameraOffsetY());

	}
	m_pCamera->UpdateCameraPosition();
}