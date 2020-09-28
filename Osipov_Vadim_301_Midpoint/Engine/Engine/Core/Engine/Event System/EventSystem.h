// EventSystem.h
#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include "EventTypes.h"
#include <unordered_map>
#include <queue>

class Event;
class EventListener;

//Static class! 
class EventSystem
{
	//Typedef's 
    typedef std::list<EventListener*> EventListenerList;
	typedef std::queue<Event*> EventQueue;
	typedef std::queue<Event*>* EventQueuePointer;
	typedef std::unordered_map<EventID, EventListenerList> ListenersHash;

	//Variables
	static ListenersHash m_listeners;
	static bool m_isQueueOne;
	//Queues
	static EventQueue m_queueOne;
	static EventQueue m_queueTwo;
	//Queue that gets executed 
	static EventQueuePointer m_pActiveQueue;
	static EventQueuePointer m_pInactiveQueue;

public:
	EventSystem() { }
	~EventSystem() { }

    
	//---------------------------------------------------------------------------------------------
	// Attaches listener for Specified Event
	//---------------------------------------------------------------------------------------------
	static void AttachListener(const EventID eventId, EventListener* pListener);
	
	//---------------------------------------------------------------------------------------------
	// Removes listener from Specified Event
	//---------------------------------------------------------------------------------------------
	static void RemoveListener(const EventID eventId, EventListener* pListener);

	//---------------------------------------------------------------------------------------------
	// Update will trigger all of the events that Event System has received this frame. 
	//---------------------------------------------------------------------------------------------
	static void Update();
	
	//---------------------------------------------------------------------------------------------
	// Used to add Event to Event System queue for Execution. 
	//---------------------------------------------------------------------------------------------
	static void AddEventForExecution(Event* pEvent);

private:
	static void TriggerEvent(const Event* pEvent);
};
#endif

