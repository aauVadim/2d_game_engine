#include "ScriptComponent.h"
#include "../../Object.h"
#include "../../Transform/Transform.h"
#include "../../../../Core/OS/Console/Logger.h"

#include "../../../../Core/Engine/Event System/EventSystem.h"
#include "../../../../Core/Engine/Event System/EventTypes.h"
#include "../../../../Core/Engine/Event System/ConcreteEvents/EventOnCreateGameObjectRequested.h"
#include "../../../../Core/Engine/Event System/ConcreteEvents/EventOnDestroyGameObjectRequested.h"
#include "../../../../Core/Engine/Event System/ConcreteEvents/EventOnCloseGameRequested.h"

#include "MovementComponent.h"

LuaPlus::LuaObject* ScriptComponent::s_metaTable = nullptr;

ScriptComponent::ScriptComponent(Object* pParent, const std::string& scriptName)
	: Component(pParent, ComponentTypes::g_kScriptComponent)
	, m_scriptName(scriptName)
	, m_objectMovePosX(0)
	, m_objectMovePosY(0)
{
}

ScriptComponent::ScriptComponent(ScriptComponent& other)
	: Component(other.GetParentObject(), ComponentTypes::g_kScriptComponent)
	, m_scriptName(other.GetScriptName())
	, m_objectMovePosX(0)
	, m_objectMovePosY(0)
{
}

ScriptComponent::~ScriptComponent()
{
}

void ScriptComponent::CreateMetatable(LuaPlus::LuaState* pState)
{
	s_metaTable = new LuaPlus::LuaObject(pState->GetGlobals().CreateTable("ScriptComponent"));
	s_metaTable->SetObject("__index", *s_metaTable);
	RegisterFunctions();
}

void ScriptComponent::DestroyMetatable()
{
	delete s_metaTable;
}

void ScriptComponent::CreateLuaObject(LuaPlus::LuaState* pState)
{
	//Shouldn't really be here, but good for now.
	m_pMovementComp = static_cast<MovementComponent*>(GetParentObject()->GetComponent(ComponentTypes::g_kMovementComponent));

    m_scriptObject = pState->BoxPointer(this);
#if LUA32
	m_scriptObject.SetMetaTable(*s_metaTable);
#elif LUA64
	m_scriptObject.SetMetatable(*s_metaTable);
#endif

	//Setting it to object id
    pState->GetGlobals().SetObject(GetParentObject()->GetObjectName().c_str(), m_scriptObject);
#if LUA32
	pState->GetGlobal(GetParentObject()->GetObjectName().c_str()).GetMetaTable().CreateTable("values");
#elif LUA64
	pState->GetGlobal(GetParentObject()->GetObjectName().c_str()).GetMetatable().CreateTable("values");
#endif
}

void ScriptComponent::CallLUAFunctionNoParams(LuaPlus::LuaState* pState, const char* funcName)
{
	//Making sure to run update on object variables. 
	UpdateObjectVariebles(m_scriptObject.GetByName("values"));

	LuaPlus::LuaObject function = pState->GetGlobal(funcName);

	//Running the function if its there.
	if (!function.IsNil() && function.IsFunction())
		((LuaPlus::LuaFunction<int>)function)(m_scriptObject);
}

void ScriptComponent::OnKeyboardInpt(LuaPlus::LuaState* pState, const char* funcName, int keyboardKey)
{
	LuaPlus::LuaObject function = pState->GetGlobal(funcName);
	//Running the function if its there.
	if (!function.IsNil() && function.IsFunction())
		((LuaPlus::LuaFunction<int>)function)(keyboardKey, m_scriptObject);
}

void ScriptComponent::OnMouseInput(LuaPlus::LuaState* pState, const char* funcName, int clickPosX, int clickPosY, int buttonNum)
{
	//Calling corresponding Lua Function
	LuaPlus::LuaObject function = pState->GetGlobal(funcName);
	//Running the function if its there.
	if (!function.IsNil() && function.IsFunction())
		((LuaPlus::LuaFunction<int>)function)(clickPosX, clickPosY, buttonNum, m_scriptObject);
}

void ScriptComponent::OnObjectClicked()
{

}

void ScriptComponent::OnCollision(LuaPlus::LuaState* pState, const char* funcName, Object* pOther)
{
	//Calling corresponding Lua Function
	LuaPlus::LuaObject function = pState->GetGlobal(funcName);
	LuaPlus::LuaObject otherObj;
	if (pOther->GetComponent(ComponentTypes::g_kScriptComponent) != nullptr)
		otherObj = static_cast<ScriptComponent*>(pOther->GetComponent(ComponentTypes::g_kScriptComponent))->GetScripObject();
	else
		otherObj.AssignNil(pState);
	//Running the function if its there.
	if (!function.IsNil() && function.IsFunction())
		((LuaPlus::LuaFunction<int>)function)(otherObj, m_scriptObject);
}

void ScriptComponent::LuaLog(int val)
{
    Logger::GetInstance()->Log("Lua Logging: %d", val);
}
//Lua Functions --- 

//TODO: Add call to movement comp
void ScriptComponent::UpdateObjectPosition(int plusX, int plusY)
{
	if (m_pMovementComp)
		m_pMovementComp->UpdateParentPosition(plusX, plusY);
}

void ScriptComponent::RotateToVector(int vecX, int vecY)
{
	if (m_pMovementComp)
		m_pMovementComp->UpdateParentRotation(vecX, vecY);
}

void ScriptComponent::CreateGameObject(int prefabID, int posX, int posY)
{
	EventSystem::AddEventForExecution(new EventOnCreateGameObjectRequested(GetParentObject(), posX, posY, prefabID));
}

void ScriptComponent::MoveToPos(int posX, int posY)
{
	if (m_pMovementComp)
		m_pMovementComp->MoveObjectToPosition(posX, posY);
}

void ScriptComponent::RegisterFunctions()
{
	//Helpers 
	s_metaTable->RegisterObjectDirect("Log", (ScriptComponent*)0, &ScriptComponent::LuaLog);

	//Object 
	s_metaTable->RegisterObjectDirect("InstantiateGameObject", (ScriptComponent*)0, &ScriptComponent::CreateGameObject);
	s_metaTable->RegisterObjectDirect("DestroyObject", (ScriptComponent*)0, &ScriptComponent::DestroyGameObject);

	//Movement 
	s_metaTable->RegisterObjectDirect("RotateToVector", (ScriptComponent*)0, &ScriptComponent::RotateToVector);
	s_metaTable->RegisterObjectDirect("UpdatePosition", (ScriptComponent*)0, &ScriptComponent::UpdateObjectPosition);
	s_metaTable->RegisterObjectDirect("MoveObjectToPos", (ScriptComponent*)0, &ScriptComponent::MoveToPos);

	//Window
	s_metaTable->RegisterObjectDirect("RequestCloseWindow", (ScriptComponent*)0, &ScriptComponent::RequestCloseWindow);
}

void ScriptComponent::UpdateObjectVariebles(LuaPlus::LuaObject table)
{
	if (!table.IsNil())
	{
		//Base object things
		table.SetBoolean("isSelected", GetParentObject()->IsSelected());
		table.SetInteger("posX", static_cast<int>(GetParentObject()->GetTransform()->GetPositionX()));
		table.SetInteger("posY", static_cast<int>(GetParentObject()->GetTransform()->GetPositionY()));

		//Movement part: 
		int movePosX = 0;
		int movePosY = 0;
		if (m_pMovementComp)
		{
			movePosX = m_pMovementComp->GetLastClickX();
			movePosY = m_pMovementComp->GetLastClickY();
		}
		table.SetInteger("movePosX", movePosX);
		table.SetInteger("movePosY", movePosY);
		table.SetInteger("rotation", static_cast<int>(GetParentObject()->GetTransform()->GetObjectRotation()));
	}
}

void ScriptComponent::DestroyGameObject()
{
	EventSystem::AddEventForExecution(new EventOnDestroyGameObjectRequested(GetParentObject()));
}

void ScriptComponent::RequestCloseWindow()
{
	EventSystem::AddEventForExecution(new EventOnCloseGameRequested());
}

