#pragma once
#include "../Event.h"
#include "../EventTypes.h"

//Just blank call to shutdown. 
class EventOnGameWindowClosed : public Event
{
public:
	EventOnGameWindowClosed() : Event(EventID::g_kEventOnGameWindowClosed) { }
};
