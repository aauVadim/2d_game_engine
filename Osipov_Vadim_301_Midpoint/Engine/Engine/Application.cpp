#include <SDL.h>
#include "ApplicationStates.h"
int main(int argc, char* argv[]) {
	// Start SDL2
	SDL_Init(SDL_INIT_EVERYTHING);

	// Create a Window in the middle of the screen
	SDL_Window *pWindow = 0;

	//Application State
	ApplicationState k_applicationState = ApplicationState::k_initializing;


	pWindow = SDL_CreateWindow("Game Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_SHOWN);

	SDL_Event event;
	SDL_PumpEvents();

	while (k_applicationState != ApplicationState::k_quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_WINDOWEVENT)
			{
				//Handling the close window event 
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					k_applicationState = ApplicationState::k_quit;
				}
			}
		}
	}

	// Cleanup and Quit
	SDL_DestroyWindow(pWindow);
	SDL_Quit();

	return 0;
}