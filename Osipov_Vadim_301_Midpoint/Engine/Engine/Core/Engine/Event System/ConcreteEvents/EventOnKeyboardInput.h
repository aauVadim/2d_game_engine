#pragma once
#include "../Event.h"
#include "../EventTypes.h"
class EventOnKeyboardInput : public Event
{
	int m_keyNum;
public:
	//Declaring this event to be MouseClick by passing it into base constructor
	EventOnKeyboardInput(int key) : Event(EventID::g_kEventOnKeyboardInput), m_keyNum(key){ }
	int GetKeyNumber() const { return m_keyNum; }
};
