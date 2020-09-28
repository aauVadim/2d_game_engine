#pragma once
#include "../Event.h"
#include "../EventTypes.h"

//Just blank call to shutdown. 
class EventOnCloseGameRequested : public Event
{
public:
	EventOnCloseGameRequested() : Event(EventID::g_kEventOnCloseGameRequested) { }
};
