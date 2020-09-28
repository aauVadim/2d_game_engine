#pragma once
#include "../Event.h"
#include "../EventTypes.h"

class Object;

class EventOnCreateGameObjectRequested : public Event
{
private:
	Object* m_pObject;
	int m_prefabID, m_posX, m_posY;
public:
	EventOnCreateGameObjectRequested(Object* pParentObject, int posX, int posY, int prefabID) : Event(EventID::g_kEventOnCreateGameObjectRequested), m_pObject(pParentObject), m_posX(posX), m_posY(posY), m_prefabID(prefabID) { }
	//---------------------------------------------------------------------------------------------
	// Returns Click position by reference
	//---------------------------------------------------------------------------------------------
	Object* GetParentObject() const { return m_pObject; }
	int GetPrefabID() const { return m_prefabID; }
	int GetPosX() const { return m_posX; }
	int GetPosY() const { return m_posY; }
};
