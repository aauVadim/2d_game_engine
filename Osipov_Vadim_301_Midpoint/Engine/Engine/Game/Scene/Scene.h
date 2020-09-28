#pragma once

#include <unordered_map>
#include "../../Core/Engine/Event System/EventListener.h"
#include "../../Core/Engine/Event System/EventTypes.h"

// This class should be responsible for having all of the game objects, 
// their components, and so on. Also game map should live here. 

class Object;
class GameField;
class LuaApi;
class PhysicsSystem;

class Scene : public EventListener
{
	//General map of game objects
	std::unordered_map<int, Object*> m_gameObjects;

	//Game Field reference
	GameField* m_pGameField;
	
	LuaApi* m_pScriptingEngine;
	PhysicsSystem* m_pPhysicsEngine;

public:
	Scene();
	~Scene();

	//---------------------------------------------------------------------------------------------
	// Returns current GameField
	//---------------------------------------------------------------------------------------------
	GameField* GetGameField() const { return m_pGameField; }
	
	//---------------------------------------------------------------------------------------------
	// Set game field that is used by the scene
	//---------------------------------------------------------------------------------------------
	void SetGameField(GameField* pGameField) { if (m_pGameField == nullptr) m_pGameField = pGameField; }

	//---------------------------------------------------------------------------------------------
	// Set Physic System that is used by the scene. 
	//---------------------------------------------------------------------------------------------
	void SetPhysicsSystem(PhysicsSystem* pCollisionSystem) { if (m_pPhysicsEngine == nullptr) m_pPhysicsEngine = pCollisionSystem; }

	//---------------------------------------------------------------------------------------------
	// Returns all GameObject's in the scene
	//---------------------------------------------------------------------------------------------
	const std::unordered_map<int, Object*>& GetGameObjectMap() const { return m_gameObjects; }

	//---------------------------------------------------------------------------------------------
	// Inherited via EventListener.
	// When events are called - Execute them. 
	//---------------------------------------------------------------------------------------------
	virtual void OnEvent(const Event* pEvent) override;

	//---------------------------------------------------------------------------------------------
	// Should be called on Application Tick. 
	//---------------------------------------------------------------------------------------------
	void Update();

public:
	void ProcessGameObject(Object* pNewObject);
	void RunObjectUpdatesAndCollision();
};
