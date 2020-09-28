#pragma once

#include "../Component.h"

class Object;
class PhysicsSystem;

class MovementComponent : public Component
{
public:
	enum class MovementState 
	{
		kStop, 
		kMoving,
	};
private:
	int m_lastClickPosX, m_lastClickPosY;
	float m_destanationPointX, m_destanationPointY;
	float m_originalRotation; 

	MovementState m_movementState;
	PhysicsSystem* m_PhysicsSystemRef;

public:
	MovementComponent(Object* pParent, float originalRotation);

	void SetPhysicsSystem(PhysicsSystem* pPSystem) { if (m_PhysicsSystemRef == nullptr) m_PhysicsSystemRef = pPSystem; }

	virtual void OnMouseClick(int clickPosX, int clickPosY) override;
	void UpdateParentRotation(int lookX, int lookY);
	//------------------------------------------------------------------------------------------------
	// Returns position X of last mouse click
	//------------------------------------------------------------------------------------------------
	int GetLastClickX() const { return m_lastClickPosX; }
	//------------------------------------------------------------------------------------------------
	// Returns position Y of last mouse click
	//------------------------------------------------------------------------------------------------
	int GetLastClickY() const { return m_lastClickPosY; }

	//------------------------------------------------------------------------------------------------
	// Will move object into POsition that Was given, with rotation to that position.
	//------------------------------------------------------------------------------------------------
	void MoveObjectToPosition(int posX, int posY);

	void UpdateParentPosition(int plusPosX, int plusPosY);

	MovementState GetObjectState() const { return m_movementState; }
	void SetObjectState(MovementState newState) { m_movementState = newState; }
};