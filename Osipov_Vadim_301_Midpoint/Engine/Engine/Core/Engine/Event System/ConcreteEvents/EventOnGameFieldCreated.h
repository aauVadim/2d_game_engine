#pragma once
#include "../Event.h"
#include "../EventTypes.h"

class GameField;

class EventOnGameFieldCreated : public Event
{
	GameField* m_pGameField;
public:
	EventOnGameFieldCreated(GameField* pGameField) : Event(EventID::k_NULL_EVENT), m_pGameField(pGameField) { }

	GameField* GetGameField() const { return m_pGameField; }
};
