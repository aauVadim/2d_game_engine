#pragma once
#include "../PhysicsSystem.h"
#include <SDL.h>

class Object;

class SDLCollisionSystem : public PhysicsSystem
{

public:
	virtual bool CheckObjectCollision(const Object* pLeft, const Object* pRight) override;

	//Not Used
	virtual void AddGOWithMovementComponent(MovementComponent*) override { }
	virtual void Update() override { }


	virtual bool CheckObjectCollision(int posX, int posY, int height, int width, const Object* pRight) override;

};