#pragma once

#include "../Component.h"
#include <string>
#include "LuaPlus.h"

class Object;
class Transform;
class Object;
class MovementComponent;

class ScriptComponent : public Component
{
	LuaPlus::LuaObject m_scriptObject;
    static LuaPlus::LuaObject* s_metaTable;
	//Helper, not used anywhere but on script creation
	std::string m_scriptName;

	MovementComponent* m_pMovementComp;

	int m_objectMovePosX, m_objectMovePosY;

public:
	ScriptComponent(Object* pParent, const std::string& scriptName);
	ScriptComponent(ScriptComponent& other);
	~ScriptComponent();
	std::string GetScriptName() const { return m_scriptName; }

	//---------------------------------------------------------------------------------------------
	// Used to GET script globals. 
	//---------------------------------------------------------------------------------------------
	LuaPlus::LuaObject& GetScripObject() { return m_scriptObject; }
	
	//---------------------------------------------------------------------------------------------
	// Used to SET script globals. 
	//---------------------------------------------------------------------------------------------
	void SetScriptObject(const LuaPlus::LuaObject& globals) { m_scriptObject = globals; }

    //---------------------------------------------------------------------------------------------
    // Lua Registration stuff
    //---------------------------------------------------------------------------------------------
    static void CreateMetatable(LuaPlus::LuaState* pState);
	static void DestroyMetatable();

    void CreateLuaObject(LuaPlus::LuaState* pState);
	// Lua Calling stuff
	void CallLUAFunctionNoParams(LuaPlus::LuaState* pState, const char* funcName);
	void OnKeyboardInpt(LuaPlus::LuaState* pState, const char* funcName, int keyboardKey);
	void OnMouseInput(LuaPlus::LuaState* pState,const char* funcName, int clickPosX, int clickPosY, int buttonNum);
	void OnObjectClicked();
	void OnCollision(LuaPlus::LuaState* pState, const char* funcName, Object* pOther);

private:
    void LuaLog(int val);
	//Lua functions
	void UpdateObjectPosition(int plusX, int plusY);
	void RotateToVector(int vecX, int vecY);
	void CreateGameObject(int prefabId, int posX, int posY);
	void MoveToPos(int posX, int posY);

	static void RegisterFunctions();
	void UpdateObjectVariebles(LuaPlus::LuaObject table);
	void DestroyGameObject();
	void RequestCloseWindow();
};
