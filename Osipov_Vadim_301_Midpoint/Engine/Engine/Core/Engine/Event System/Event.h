// Event.h
#ifndef ENGINEEVENT_H
#define ENGINEEVENT_H

#include "EventTypes.h"

class Event
{
    EventID m_eventType;

public:
	//Events are defaulted to null, to reduce user mistakes
    Event(const EventID eventType = EventID::k_NULL_EVENT) : m_eventType(eventType) { }
	virtual ~Event() { };

    const EventID GetEngineEventId() const { return m_eventType; }
};

#endif //ENGINEEVENT_H
