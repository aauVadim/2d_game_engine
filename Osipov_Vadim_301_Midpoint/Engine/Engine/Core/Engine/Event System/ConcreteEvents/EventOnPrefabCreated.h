#pragma once
#include "../Event.h"
#include "../EventTypes.h"

class Object;

class EventOnPrefabCreated : public Event
{
private:
	Object* m_pParentObject;
public:
	EventOnPrefabCreated(Object* pObject) : Event(EventID::g_kEventOnPrefabCreated), m_pParentObject(pObject) { }
	//---------------------------------------------------------------------------------------------
	// Returns Click position by reference
	//---------------------------------------------------------------------------------------------
	Object* GetObject() const { return m_pParentObject; }
};
