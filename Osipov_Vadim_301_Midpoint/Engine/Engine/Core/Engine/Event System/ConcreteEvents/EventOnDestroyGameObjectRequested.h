#pragma once
#include "../Event.h"
#include "../EventTypes.h"

class Object;

class EventOnDestroyGameObjectRequested : public Event
{
private:
	Object* m_pObject;
public:
	EventOnDestroyGameObjectRequested(Object* pParentObject) : Event(EventID::g_kEventOnDestroyGameObjectRequested), m_pObject(pParentObject) { }
	//---------------------------------------------------------------------------------------------
	// Returns Click position by reference
	//---------------------------------------------------------------------------------------------
	Object* GetObject() const { return m_pObject; }
};
