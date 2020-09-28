#pragma once
#include <vector>
#include <LuaPlus.h>

#include "../Engine/Event System/EventListener.h"

class ScriptComponent;
class Object;

class LuaApi : public EventListener
{
	std::vector<ScriptComponent*> m_scriptComponents;
	LuaPlus::LuaState* m_pStateOwner; 

public:
	LuaApi();
	~LuaApi();

	//---------------------------------------------------------------------------------------------
	// Registers a script and calls Start function on it.
	//---------------------------------------------------------------------------------------------
	bool RegisterGOWithScript(Object* pGameObject);
	void RemoveGOWithScriptComponent(Object* pGameObject);

	//---------------------------------------------------------------------------------------------
	// Update function. Calls Update on all scripts
	//---------------------------------------------------------------------------------------------
	void Update();

	void OnObjectsCollided(Object* pLeft, Object* pRight);


private:

	//---------------------------------------------------------------------------------------------
	// Validates return of the function. if 0 - continue
	//---------------------------------------------------------------------------------------------
	bool ValidateReturn(const char* file, int ret);

	//---------------------------------------------------------------------------------------------
	// Calls a given function
	//---------------------------------------------------------------------------------------------
	bool CallFunction(const char* functionName, ScriptComponent* pScriptComp);

	
	virtual void OnEvent(const Event* pEvent) override;
	bool DoLuaFile(ScriptComponent* pScriptComp);
	void SafeDeleteScriptComp(ScriptComponent* pScriptComp);
};
