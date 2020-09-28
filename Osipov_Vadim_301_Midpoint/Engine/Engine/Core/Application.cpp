//Name: Vadim Osipov

//#include "vld.h"
#include <SDL.h> // For some reason SDL needs to have it here. 
#include"Engine\Engine.h"

#include "UtilMacros.h"

int main(int, char**)
{
	//Creating Engine class
	Engine* pEngine = new Engine();
	//If we passed Initialize then run updates
	if (pEngine->Initialize())
	{
		//App Loop 
		pEngine->Update();
	}
	//Nuke the thing
	_SAFE_DELETE_(pEngine);
	return 0;
}