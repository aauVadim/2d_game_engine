#include "LuaApi.h"
#include "../OS/Console/Logger.h"
#include "../../Game/Object/Component/Concrete/ScriptComponent.h"
#include "../../Game/Object/Object.h"

#include "../Engine/Event System/ConcreteEvents/EventOnKeyboardInput.h"
#include "../Engine/Event System/ConcreteEvents/EventOnDestroyGameObjectRequested.h"
#include "../Engine/Event System/ConcreteEvents/EventOnMouseClick.h"
#include "../Engine/Event System/EventSystem.h"

#include "../Factory/Resource/ResourseLoader.h"

using namespace LuaPlus;

LuaApi::LuaApi()
	: m_pStateOwner(LuaState::Create(true))
{
    ScriptComponent::CreateMetatable(m_pStateOwner);

	//Registering for events
	EventSystem::AttachListener(EventID::g_kEventOnKeyboardInput, this);
	EventSystem::AttachListener(EventID::g_kEventOnMouseClick, this);
}

LuaApi::~LuaApi()
{
	//Making sure to destroy scripts metatable before state goes
	ScriptComponent::DestroyMetatable();

	m_scriptComponents.clear();
	if(m_pStateOwner)
		LuaState::Destroy(m_pStateOwner);
}

bool LuaApi::RegisterGOWithScript(Object* pGameObject)
{
	//Retrieving the script component
	ScriptComponent* pScriptComp = (ScriptComponent*)pGameObject->GetComponent(ComponentTypes::g_kScriptComponent);

	pScriptComp->CreateLuaObject(m_pStateOwner);

	//We do not have any compiler errors, proceed
	if (CallFunction("Start", pScriptComp))
	{
		m_scriptComponents.push_back(pScriptComp);
		Logger::GetInstance()->Log("Added ( %s ) to runtime scripts", pScriptComp->GetScriptName().c_str());
		return true;
	}
	else
	{
		Logger::GetInstance()->LogError("Could not add ( %s ) to runtime scripts, something must have gone wrong", pScriptComp->GetScriptName().c_str());
	}

	//Unhappy place. 
	return false;
}

void LuaApi::RemoveGOWithScriptComponent(Object* pGameObject)
{
	ScriptComponent* pScriptComp = (ScriptComponent*)pGameObject->GetComponent(ComponentTypes::g_kScriptComponent);
	//Lets see if it has children
	if (!pGameObject->GetChildrenVector().empty())
	{
		//Get the vector
		auto children = pGameObject->GetChildrenVector();
		for (size_t i = 0; i < children.size(); ++i)
		{
			//See if we have the component or not, if so - remove it too
			if (children[i]->GetComponent(ComponentTypes::g_kScriptComponent) != nullptr)
			{
				ScriptComponent* pChildScript = ((ScriptComponent*)children[i]->GetComponent(ComponentTypes::g_kScriptComponent));
				CallFunction("Destroy", pChildScript);
				SafeDeleteScriptComp(pChildScript);
			}
		}
	}
	//Delete actual object now 
	CallFunction("Destroy", pScriptComp);
	SafeDeleteScriptComp(pScriptComp);
}

void LuaApi::Update()
{
	if (m_scriptComponents.size() > 0)
	{
		//Loop over script Update functions
		for (ScriptComponent* pScript : m_scriptComponents)
		{
			CallFunction("Update", pScript);
		}
	}
}

void LuaApi::OnObjectsCollided(Object* pLeft, Object* pRight)
{
	//pLeft HAS TO HAVE Script component for this function to be called. 
	//So lets call pLeft collision function first. 
	ScriptComponent* pLeftScript = static_cast<ScriptComponent*>(pLeft->GetComponent(ComponentTypes::g_kScriptComponent));
	if (DoLuaFile(pLeftScript))
	{
		pLeftScript->OnCollision(m_pStateOwner, "OnCollision", pRight);
	}
}

bool LuaApi::CallFunction(const char* functionName, ScriptComponent* pScriptComp)
{

	if (DoLuaFile(pScriptComp))
	{
		pScriptComp->CallLUAFunctionNoParams(m_pStateOwner, functionName);
		return true;
	}
	return false;
}

void LuaApi::OnEvent(const Event* pEvent)
{
	if (pEvent->GetEngineEventId() == EventID::g_kEventOnKeyboardInput)
	{
		EventOnKeyboardInput* pKeyboardInput = (EventOnKeyboardInput*)pEvent;
		if (!m_scriptComponents.empty())
		{
			//Loop over script Update functions
			for (ScriptComponent* pScript : m_scriptComponents)
			{
				if (DoLuaFile(pScript))
				{
					pScript->OnKeyboardInpt(m_pStateOwner, "OnKeyboardInput", pKeyboardInput->GetKeyNumber());
				}
			}
		}
	}
	else if (pEvent->GetEngineEventId() == EventID::g_kEventOnMouseClick)
	{
		EventOnMouseClick* pMouseInputEvent = (EventOnMouseClick*)pEvent;

		if (!m_scriptComponents.empty())
		{
			//Loop over script Update functions
			for (ScriptComponent* pScript : m_scriptComponents)
			{
				if (DoLuaFile(pScript))
				{
					int clickX, clickY;
					pMouseInputEvent->GetClickPosition(clickX, clickY);
					pScript->OnMouseInput(m_pStateOwner, "OnMouseInput", clickX, clickY, static_cast<int>(pMouseInputEvent->GetButton()));
				}
			}
		}
	}
}
// Utility, calls a lua file with validation of return
bool LuaApi::DoLuaFile(ScriptComponent* pScriptComp)
{
	std::string scriptFilePath(ResourseLoader::GetScriptsLocation() + pScriptComp->GetScriptName().c_str());
	return ValidateReturn(scriptFilePath.c_str(), m_pStateOwner->DoFile(scriptFilePath.c_str()));
}

//Validates file read, prints out message if it doesn't like something
bool LuaApi::ValidateReturn(const char* file, int ret)
{
	if (!ret)
		return true;
	else
	{
		if (ret)
			Logger::GetInstance()->LogError("File Read: ( %s ) != 0. Return: %d\nScript has compiler errors", file, ret);
		else
			Logger::GetInstance()->LogError("File Read: ( %s ) != 0. Return: %d, Unknown error. Fix it", file, ret);
	}

	return false;
}

void LuaApi::SafeDeleteScriptComp(ScriptComponent* pScriptComp)
{
	for (auto iter = m_scriptComponents.begin(); iter != m_scriptComponents.end();)
	{
		auto tempIt = iter;
		//If we have it
		if (*tempIt == pScriptComp)
		{
			iter = m_scriptComponents.erase(tempIt);
			break;
		}
		else
			++iter;
	}
}

