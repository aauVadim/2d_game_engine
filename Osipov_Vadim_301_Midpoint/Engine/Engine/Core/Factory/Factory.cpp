
#include "Factory.h"
//SDL Renderer
#include "../Renderer/Implementation/SDL/SDLRenderer.h"
#include "../OS/Window/Implementation/SDLWindow.h"
#include "../JSON/json_api.h"
#include "../UtilMacros.h"

#include "../OS/Console/Logger.h"
#include "../Engine/Engine.h"

#include "../Physics/PhysicsSystem.h"
#include "../Physics/Implementation/SDLCollisionSystem.h"

#include "../../Game/Scene/Scene.h"
#include "../../Game/Object/Object.h"
#include "../../Game/Object/Transform/Transform.h"
#include "../../Game/GameField/GameField.h"

//Components
#include "../../Game/Object/Component/Component.h"

#include "../Engine/Event System/EventSystem.h"
#include "../Engine/Event System/EventTypes.h"
#include "../Engine/Event System/Event.h"
#include "../Engine/Event System/ConcreteEvents/EventOnPrefabCreated.h"
#include "../Engine/Event System/ConcreteEvents/EventOnKeyboardInput.h"
#include "../Engine/Event System/ConcreteEvents/EventOnObjectCreated.h"
#include "../Engine/Event System/ConcreteEvents/EventOnCreateGameObjectRequested.h"

//Timer
#include "../Engine/FPSTimer/FPSTimer.h"
#include "../Engine/FPSTimer/Implementation/SDL_FPSTimer.h"

#include "Resource/ResourseLoader.h"

#include <iostream>
#include <fstream>

IRendererDevice* Factory::m_pCurrentRenderer = nullptr;
IWindow* Factory::m_pWindow = nullptr;

Factory::Factory(PlatformConfiguration platformConfiguration, const char* confFile)
	: m_platformConfig(platformConfiguration)
	, m_prefabsSpawned(0)
	, m_pJSONLoader(new json_api(this))
	, m_configFileLocation(confFile)
{
	EventSystem::AttachListener(EventID::g_kEventOnPrefabCreated, this);
	EventSystem::AttachListener(EventID::g_kEventOnKeyboardInput, this);
	EventSystem::AttachListener(EventID::g_kEventOnCreateGameObjectRequested, this);

	std::ifstream confFileRead(m_configFileLocation);
	//Reading and setting configs
	if (confFileRead.is_open())
	{
		ResourseLoader::SetConfigs(m_pJSONLoader->ReadConfigFile(confFileRead));
		Logger::GetInstance()->Log("Success reading ( %s )", m_configFileLocation);
	}
	else
		Logger::GetInstance()->LogError("Factory: Could not open config file: ( %s )", m_configFileLocation);

	confFileRead.close();
}

Factory::~Factory()
{
	_SAFE_DELETE_(m_pJSONLoader);
	for (auto pGO : m_gamePrefabs)
	{
		_SAFE_DELETE_(pGO.second);
	}
	m_gamePrefabs.clear();

	ResourseLoader::ShutDown();
}

FPSTimer* Factory::CreateFPSTimer()
{
	switch (m_platformConfig)
	{
	case PlatformConfiguration::g_kSDL:
		return new SDLFPStimer();
		break;
	case PlatformConfiguration::g_kDirectX:
		return nullptr;
		break;
	case PlatformConfiguration::g_kOpenGL:
		return nullptr;
		break;
	default:
		return nullptr;
		break;
	}
}

IRendererDevice* Factory::CreateRendererDevice()
{
	switch (m_platformConfig)
	{
	case PlatformConfiguration::g_kSDL:
		m_pCurrentRenderer = new SDLRenderer();
		m_pCurrentRenderer->CreateRenderDevice(m_pWindow);
		return m_pCurrentRenderer;
		break;
	case PlatformConfiguration::g_kDirectX:
		return m_pCurrentRenderer;
		break;
	case PlatformConfiguration::g_kOpenGL:
		return m_pCurrentRenderer;
		break;
	default:
		return m_pCurrentRenderer;
		break;
	}
}

IWindow* Factory::CreateEngineWindow()
{
	switch (m_platformConfig)
	{
	case PlatformConfiguration::g_kSDL:
		m_pWindow = new SDLWindow();
		//Did we get it? 
		if (m_pWindow->Initialize())
			return m_pWindow;
		else
			Logger::GetInstance()->Log("Something went terrible wrong with window init.", Logger::LogType::k_Error);
			return nullptr;
		break;
	case PlatformConfiguration::g_kDirectX:
		return m_pWindow;
		break;
	case PlatformConfiguration::g_kOpenGL:
		return m_pWindow;
		break;
	default:
		return m_pWindow;
		break;
	}
}
 

bool Factory::CreateScene()
{
	//reading/opening a file
	std::ifstream fileOpen(ResourseLoader::GetMapFile().c_str());

	//Getting Game Objects out
	if (fileOpen.is_open())
	{
		if (!m_pJSONLoader->ReadTiledFile(fileOpen))
		{
			//Bailing out with error, useful one? 
			Logger::GetInstance()->LogError("Something went wrong with parsing file: ( %s )", ResourseLoader::GetMapFile().c_str());
			fileOpen.close();
			return false;
		}
	}
	else
		Logger::GetInstance()->LogError("Could not open ( %s ) FIXME!", ResourseLoader::GetMapFile().c_str());

	//Closing the file
	fileOpen.close();
	return true;
}

void Factory::CreateGameObject(Object* pParent, int prefabID)
{
	//Copy constructing a thing
	Object* pNewGO = new Object(*m_gamePrefabs[prefabID]);
	//Hard setting objects new position for object
	pNewGO->GetTransform()->SetPosition(pParent->GetTransform()->GetPositionX(), pParent->GetTransform()->GetPositionY());
	//TODO: Figure out how to make this nicer
	pNewGO->SetNewObjectID(1000 + prefabID * m_prefabsSpawned);
	//Adding a child to a parent. 
	pParent->AddChildObject(pNewGO);
	//Making sure to let the child know about its parent. 
	pNewGO->SetParentObject(pParent);
	//Calling it a day
	EventSystem::AddEventForExecution(new EventOnObjectCreated(pNewGO));
	Logger::GetInstance()->Log("Created new Game Object ( %s ) at X: ( %d ) Y ( %d ) from prefab: ( %s )", pNewGO->GetObjectName().c_str(), pNewGO->GetTransform()->GetPositionX(), pNewGO->GetTransform()->GetPositionY(), m_gamePrefabs[10]->GetObjectName().c_str());
	m_prefabsSpawned++;
}

void Factory::OnEvent(const Event* pEvent)
{
	if (pEvent->GetEngineEventId() == EventID::g_kEventOnPrefabCreated)
	{
		Object* pGameObject = (Object*)((EventOnPrefabCreated*)pEvent)->GetObject();
		m_gamePrefabs.emplace(pGameObject->GetObjectID(), pGameObject);
	}
	else if (pEvent->GetEngineEventId() == EventID::g_kEventOnCreateGameObjectRequested)
	{
		EventOnCreateGameObjectRequested* pCreateRequest = (EventOnCreateGameObjectRequested*)pEvent;
		CreateGameObject(pCreateRequest->GetParentObject(), pCreateRequest->GetPrefabID());
	}
}
