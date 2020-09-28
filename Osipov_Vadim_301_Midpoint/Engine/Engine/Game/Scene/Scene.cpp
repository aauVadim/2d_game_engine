
#include "Scene.h"

#include "../../Core/OS/Console/Logger.h"

#include "../../Core/UtilMacros.h"

#include "../../Core/Engine/Event System/Event.h"
#include "../../Core/Engine/Event System/EventSystem.h"
#include "../../Core/Engine/Event System/ConcreteEvents/EventOnMouseClick.h"
#include "../../Core/Engine/Event System/ConcreteEvents/EventOnObjectCreated.h"
#include "../../Core/Engine/Event System/ConcreteEvents/EventOnKeyboardInput.h"
#include "../../Core/Engine/Event System/ConcreteEvents/EventOnDestroyGameObjectRequested.h"
#include "../../Core/Engine/Event System/ConcreteEvents/EventOnMarqueeSelect.h"


#include "../../Core/Physics/PhysicsSystem.h"

#include "../Object/Object.h"

#include "../../Core/Factory/Factory.h"
#include "../GameField/GameField.h"

#include "../../Core/LUA/LuaApi.h"

Scene::Scene()
	: m_pGameField(nullptr)
	, m_pPhysicsEngine(nullptr)
	, m_pScriptingEngine(new LuaApi)
{
	//Subscribing for events
	EventSystem::AttachListener(EventID::g_kEventOnMouseClick, this);
	EventSystem::AttachListener(EventID::g_kEventOnObjectCreated, this);
	EventSystem::AttachListener(EventID::g_kEventOnKeyboardInput, this);
	EventSystem::AttachListener(EventID::g_kEventOnDestroyGameObjectRequested, this);
	EventSystem::AttachListener(EventID::g_kEventOnMarqueeSelect, this);
}

Scene::~Scene()
{
	for (auto pGO : m_gameObjects)
	{
		delete pGO.second;
	}
	m_gameObjects.clear();

	//Destroying managers
	_SAFE_DELETE_(m_pGameField);
	_SAFE_DELETE_(m_pScriptingEngine);
	_SAFE_DELETE_(m_pPhysicsEngine);
	
}

void Scene::Update()
{
	//Updating our scripts runner
	m_pScriptingEngine->Update();

	//Updating Physics
	m_pPhysicsEngine->Update();

	if (!m_gameObjects.empty())
	{
		//If we have collision system
		RunObjectUpdatesAndCollision();
	}
}

void Scene::ProcessGameObject(Object* pNewObject)
{
	//Only if this is an object instantiated from JSON and not as prefab. 
	if(pNewObject->GetParentGameObject() == nullptr)
		m_gameObjects.emplace(pNewObject->GetObjectID(), pNewObject);

	//If we have a script component on the object and we have a lua system 
	if (pNewObject->GetComponent(ComponentTypes::g_kScriptComponent) != nullptr)
	{
		if(m_pScriptingEngine != nullptr)
			m_pScriptingEngine->RegisterGOWithScript(pNewObject);
	}
	//If we have a movement component, and Physics system add GO to Physics system
	if (pNewObject->GetComponent(ComponentTypes::g_kMovementComponent) != nullptr)
	{
		if(m_pPhysicsEngine != nullptr)
			m_pPhysicsEngine->AddGOWithMovementComponent((MovementComponent*)pNewObject->GetComponent(ComponentTypes::g_kMovementComponent));
	}
}

void Scene::RunObjectUpdatesAndCollision()
{
	for (auto pLeft : m_gameObjects)
	{
		//Update game object
		pLeft.second->Update();
		//and if we have collision system, check collisions. 
		if (m_pPhysicsEngine != nullptr)
		{
			for (auto pRight : m_gameObjects)
			{
				//not checking itself
				if(pLeft == pRight)
					continue;

				//Checking base collision 
				if (m_pPhysicsEngine->CheckObjectCollision(pLeft.second, pRight.second))
				{
					if (pLeft.second->GetComponent(ComponentTypes::g_kScriptComponent) != nullptr)
					{
						m_pScriptingEngine->OnObjectsCollided(pLeft.second, pRight.second);
					}
				}
			
				//Checking collision fore every left child
				if (!pLeft.second->GetChildrenVector().empty())
				{
					for (auto pLeftChild : pLeft.second->GetChildrenVector())
					{
						if(m_pPhysicsEngine->CheckObjectCollision(pLeftChild, pRight.second))
							Logger::GetInstance()->Log("Child Object ( %s ) of ( %s ) collided with Object ( %s )", pLeftChild->GetObjectName().c_str(), pLeft.second->GetObjectName().c_str(), pRight.second->GetObjectName().c_str());
					}
				}
			}
		}
	}
}

//Engine event handler. 
void Scene::OnEvent(const Event* pEvent)
{
	switch (pEvent->GetEngineEventId())
	{
	case EventID::g_kEventOnObjectCreated:
		if (((EventOnObjectCreated*)pEvent)->GetObject())
		{
			ProcessGameObject((Object*)((EventOnObjectCreated*)pEvent)->GetObject());
		}
		break;
	case EventID::g_kEventOnMouseClick:
		if (((EventOnMouseClick*)pEvent)->GetButton() == EventOnMouseClick::MouseButton::kRMB)
		{
			int posX, posY;
			((EventOnMouseClick*)pEvent)->GetClickPosition(posX, posY);
			for (auto pObj : m_gameObjects)
			{
				//This seems a little shady. TODO: Fix me
				if(pObj.second->IsSelected())
					pObj.second->OnMouseDown(posX, posY);

				//If object has children
				if (!pObj.second->GetChildrenVector().empty())
				{
					//Notify them too
					for (auto pObjChild : pObj.second->GetChildrenVector())
					{
						if(pObjChild->IsSelected())
							pObjChild->OnMouseDown(posX, posY);
					}
				}
			}
			break;
		}
	case EventID::g_kEventOnMarqueeSelect:
		int x, y, h, w;
		((EventOnMarqueeSelect*)pEvent)->GetBoxData(x, y, h, w);
		for (auto pObj : m_gameObjects)
		{
			pObj.second->SetSelected(m_pPhysicsEngine->CheckObjectCollision(x, y, h, w, pObj.second));

			//If object has children
			if (!pObj.second->GetChildrenVector().empty())
			{
				//Notify them too
				for (auto pObjChild : pObj.second->GetChildrenVector())
				{
					pObjChild->SetSelected(m_pPhysicsEngine->CheckObjectCollision(x, y, h, w, pObjChild));
				}
			}

		}
		break;
	case EventID::g_kEventOnDestroyGameObjectRequested:
		int objID = ((EventOnDestroyGameObjectRequested*)pEvent)->GetObject()->GetObjectID();
		//Letting lua guys know to nuke it. 
		if (m_gameObjects[objID] != nullptr && m_gameObjects[objID]->GetComponent(ComponentTypes::g_kScriptComponent) != nullptr)
			m_pScriptingEngine->RemoveGOWithScriptComponent(m_gameObjects[objID]);
		//Deleting actual object
		delete m_gameObjects[objID];
		//Erasing record
		m_gameObjects.erase(objID);
		break;
	}
}