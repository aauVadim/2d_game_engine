//Engine.cpp

#include "Engine.h"

#include "FPSTimer/FPSTimer.h"

#include "../Constants.h"
#include "../Config.h"
#include "../OS/Console/Logger.h"
#include "../UtilMacros.h"

#include "Event System/EventSystem.h"
#include "Event System/ConcreteEvents/EventOnGameWindowClosed.h"
#include "Event System/ConcreteEvents/EventOnSceneCreated.h"
#include "Event System/ConcreteEvents/EventOnKeyboardInput.h"

#include "../JSON/json_api.h"
#include "../../Game/Scene/Scene.h"
#include "../LUA/LuaApi.h"

#include "../Factory/Resource/Config/Config.h"

//TODO: Pull this out of constructor
Engine::Engine()
	: m_applicationState(ApplicationState::k_initializing)
	, m_pFactory(new Factory(PlatformConfiguration::g_kSDL, "Game/Source/config/config.json"))
	, m_pTimer(m_pFactory->CreateFPSTimer())				//Creating FPS Timer 
	, m_pWindow(m_pFactory->CreateEngineWindow())			//Creating Window
	, m_pRenderer(m_pFactory->CreateRendererDevice())		//Creating Rendering Device
	, m_pScene(nullptr)
{
	EventSystem::AttachListener(EventID::g_kEventOnGameWindowClosed, this);
	EventSystem::AttachListener(EventID::g_kEventOnSceneCreated, this);
	EventSystem::AttachListener(EventID::g_kEventOnKeyboardInput, this);
	EventSystem::AttachListener(EventID::g_kEventOnCloseGameRequested, this);
}

Engine::~Engine()
{
	_SAFE_DELETE_(m_pTimer);

	//Deleting things
	_SAFE_DELETE_(m_pWindow);
	_SAFE_DELETE_(m_pRenderer);
	_SAFE_DELETE_(m_pFactory);
	_SAFE_DELETE_(m_pScene);

	//Logger(singleton) - Exception
	Logger::GetInstance()->Shutdown();
}

bool Engine::Initialize()
{
	if (m_pFactory->CreateScene())
		return true;

	return false;
}

void Engine::Update()
{

	while (m_applicationState != ApplicationState::k_quit)
	{
		//Trigger all event at the beginning of the frame. 
		EventSystem::Update();

		//We only want to run those classes when game is running
		if (m_applicationState == ApplicationState::k_running)
		{
			//Updates
			m_pWindow->Update();
			m_pScene->Update();
			m_pTimer->Update();
		}

		//THREAD BLOCKING ---- VSync
		m_pRenderer->Update();
	}
}

void Engine::OnEvent(const Event* pEvent)
{
	switch (pEvent->GetEngineEventId())
	{
	case EventID::g_kEventOnGameWindowClosed:
		m_applicationState = ApplicationState::k_quit;
		break;
	case EventID::g_kEventOnKeyboardInput:
		if(((EventOnKeyboardInput*)pEvent)->GetKeyNumber() == 27)
			m_applicationState = ApplicationState::k_quit;
		break;
	case EventID::g_kEventOnSceneCreated:
		m_pScene = ((EventOnSceneCreated*)pEvent)->GetScene();
		m_applicationState = ApplicationState::k_running;
		break;
	// Just bail, for now
	case EventID::g_kEventOnCloseGameRequested:
		m_applicationState = ApplicationState::k_quit;
		break;
	}
}
