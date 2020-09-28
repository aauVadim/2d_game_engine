#pragma once

#include "../Config.h"
#include "../Renderer/Virtuals/IRendererDevice.h"
#include "../OS/Window/Virtuals/IWindow.h"
#include "../../Game/Object/Component/ComponentTypes.h"

#include "../Engine/Event System/EventListener.h"


#include <string>
#include <unordered_map>


//TODO: Create a Window abstract
//TODO: Create a Texture abstract

class Engine;
class json_api;
class Scene;
class FPSTimer;

class Component;

class Object;

class Factory : public EventListener
{
public:
	static IRendererDevice* m_pCurrentRenderer;
	static IWindow* m_pWindow;

private:
	typedef std::unordered_map<int, Object*> Prefabs; 
	//Storing possible prefabs
	Prefabs m_gamePrefabs;
	int m_prefabsSpawned;
	//------------------------------------------------------------------------------------------------
	// Data Readers
	//------------------------------------------------------------------------------------------------
	json_api* m_pJSONLoader;
	const char* m_configFileLocation;

	PlatformConfiguration m_platformConfig;
public:
	Factory(PlatformConfiguration platformConfiguration, const char* confFile);
	~Factory();

	FPSTimer* CreateFPSTimer();

	//------------------------------------------------------------------------------------------------
	// Creates RenderDevice for current config. 
	//------------------------------------------------------------------------------------------------
	IRendererDevice* CreateRendererDevice();

	//------------------------------------------------------------------------------------------------
	// Creates OS Window for current config
	//------------------------------------------------------------------------------------------------
	IWindow* CreateEngineWindow();

	//------------------------------------------------------------------------------------------------
	// Makes Scene, uses Resource Loaders configs
	//------------------------------------------------------------------------------------------------
	bool CreateScene();

	//------------------------------------------------------------------------------------------------
	// Makes GameObject's
	//------------------------------------------------------------------------------------------------
	void CreateGameObject(Object* pParent, int prefabID);

	PlatformConfiguration GetCurrentConfiguration() const { return m_platformConfig; }

	virtual void OnEvent(const Event* pEvent) override;
};