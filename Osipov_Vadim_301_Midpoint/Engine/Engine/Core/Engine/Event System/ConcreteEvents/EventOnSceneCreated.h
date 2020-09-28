#pragma once
#include "../Event.h"
#include "../EventTypes.h"

class Scene;

class EventOnSceneCreated : public Event
{
	Scene* m_pScene;
public:
	EventOnSceneCreated(Scene* pScene) : Event(EventID::g_kEventOnSceneCreated), m_pScene(pScene) { }

	Scene* GetScene() const { return m_pScene; }
};
