#pragma once
#include "../Event.h"
#include "../EventTypes.h"

class Object;

class EventOnObjectCreated : public Event
{
private:
	Object* m_pObject;
public:
	EventOnObjectCreated(Object* pObject) : Event(EventID::g_kEventOnObjectCreated), m_pObject(pObject) { }
	~EventOnObjectCreated() { }
	//---------------------------------------------------------------------------------------------
	// Returns Click position by reference
	//---------------------------------------------------------------------------------------------
	Object* GetObject() const { return m_pObject; }
};
