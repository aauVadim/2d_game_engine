#pragma once

#include "../PhysicsSystem.h"

#include <vector>
#include "Box2D.h"

class Object;
class MovementComponent;

struct Box2DComponent
{
	b2Body* m_pObj2dBody;
	b2BodyDef m_obj2dBodyDef;
	b2PolygonShape m_b2PolygonShape;
	b2FixtureDef m_b2FixtureDef;

	MovementComponent* m_movementCompRef;
	Box2DComponent() : m_movementCompRef(nullptr), m_pObj2dBody(nullptr) { }
	~Box2DComponent() { if (m_pObj2dBody) m_pObj2dBody->GetWorld()->DestroyBody(m_pObj2dBody); }
};


class Box2DPhisycs : public PhysicsSystem
{

	b2World* m_pB2DWorld; 

	std::vector<Box2DComponent*> m_dynamicObjects; 
public:
	Box2DPhisycs();
	~Box2DPhisycs();

	virtual void AddGOWithMovementComponent(MovementComponent* pMovementComp) override;

	//Muted for now 
	virtual void Update() override;
	virtual bool CheckObjectCollision(const Object*, const Object*) override { return false; }
	virtual bool CheckObjectCollision(int, int, int, int, const Object*) override { return false; }
};