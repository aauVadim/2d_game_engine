// EventSystem.cpp
#include "EventSystem.h"
#include "Event.h"
#include "EventListener.h"

#include "../../UtilMacros.h"

bool EventSystem::m_isQueueOne = true;
//Pointers
EventSystem::EventQueuePointer EventSystem::m_pActiveQueue = nullptr;
EventSystem::EventQueuePointer EventSystem::m_pInactiveQueue = nullptr;

//Queues
EventSystem::EventQueue EventSystem::m_queueOne;
EventSystem::EventQueue EventSystem::m_queueTwo;

//Listeners
EventSystem::ListenersHash EventSystem::m_listeners;

void EventSystem::AttachListener(const EventID eventId, EventListener* pListener)
{
	m_listeners[eventId].push_back(pListener);
}

void EventSystem::RemoveListener(const EventID eventId, EventListener* pListener)
{
	// find the list we care about
	auto findIt = m_listeners.find(eventId);
	if (findIt == m_listeners.end())
		return;

	// loop through the list and find the listener to remove
	EventListenerList& eventListenerList = findIt->second;
	eventListenerList.remove(pListener);
}

// event management
void EventSystem::Update()
{
	// Switching Queue boolean and Queue pointer
	m_isQueueOne = !m_isQueueOne;
	m_pActiveQueue = (m_isQueueOne) ? (&m_queueOne) : (&m_queueTwo);

	//Safety check
	if (!m_pActiveQueue->empty())
	{
		//Maybe lame way Iterating trough the queue
		//Had to make it Unsigned - VS was giving me some errors 
		for (unsigned int i = 0; i < m_pActiveQueue->size() + 1; ++i)
		{
			//Getting first in the line element
			TriggerEvent(m_pActiveQueue->front());
			m_pActiveQueue->pop();
		}
	}
}

void EventSystem::TriggerEvent(const Event* pEvent)
{
	// find the list we care about
	auto findIt = m_listeners.find(pEvent->GetEngineEventId());
	if (findIt == m_listeners.end())
		return;

	// send the event to every listener
	for (auto* pEventListener : findIt->second)
	{
		pEventListener->OnEvent(pEvent);
	}
	_SAFE_DELETE_(pEvent);
}

void EventSystem::AddEventForExecution(Event* pEvent)
{
	//Pushing things to Inactive Queue
	m_pInactiveQueue = (m_isQueueOne) ? (&m_queueTwo) : (&m_queueOne);
	m_pInactiveQueue->push(pEvent);
}




