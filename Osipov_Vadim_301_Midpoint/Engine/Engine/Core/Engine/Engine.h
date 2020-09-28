#pragma once

#include <vector>

#include"../ApplicationStates.h"

#include "../Renderer/Virtuals/IRendererDevice.h"
#include "../Factory/Factory.h"

#include "../OS/Window/Virtuals/IWindow.h"
#include "Event System/EventListener.h"

class Logger;
class Scene;
class LuaApi;
class EngineComponent;
class FPSTimer;

class Engine : public EventListener
{
	//Storing Abstract Factory 
	Factory* m_pFactory;

	//Runners 
	FPSTimer* m_pTimer;
	IWindow* m_pWindow;
	IRendererDevice* m_pRenderer;
	Scene* m_pScene;

	//Variables
	ApplicationState m_applicationState;

public:
	Engine();
	~Engine();
	
	//---------------------------------------------------------------------------------------------
	// Used to Initialize Game Engine, will add all necessary Engine Components
	//---------------------------------------------------------------------------------------------
	bool Initialize();

	//---------------------------------------------------------------------------------------------
	// Runs Engine Updates, should be called every platform tick. 
	//---------------------------------------------------------------------------------------------
	void Update();

	//---------------------------------------------------------------------------------------------
	// System Event Receiver
	//---------------------------------------------------------------------------------------------
	virtual void OnEvent(const Event* pEvent) override;
};