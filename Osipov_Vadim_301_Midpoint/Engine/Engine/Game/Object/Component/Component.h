#pragma once

#include "ComponentTypes.h"
#include <unordered_map>
#include <string>

class Object;

class Component
{
private:
	Object* m_pGameObject;
	ComponentTypes m_type;
public:
	Component(Object* pGameObject, ComponentTypes type) : m_pGameObject(pGameObject), m_type(type) { }
	virtual ~Component() { }
	ComponentTypes GetCompType() const { return m_type; }
	Object* GetParentObject() const { return m_pGameObject; }

	//---------------------------------------------------------------------------------------------
	// Virtual
	//---------------------------------------------------------------------------------------------
	virtual void UpdateComponent() { }
	virtual void OnMouseClick(int, int) { }		
};