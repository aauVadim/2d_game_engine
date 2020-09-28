#pragma once

class Object;
class MovementComponent;

//---------------------------------------------------------------------------------------------
// Idea behind this class is that I can add physics system later. and I even started with Box2D
// Because we were short on time - I did not wanted to submit half ass Physics System. So I stopped and left it with SDL.  
//---------------------------------------------------------------------------------------------

//Class that represents collision system of Engine
class PhysicsSystem
{
public:
	virtual void AddGOWithMovementComponent(MovementComponent* pObject) = 0;
	//Used to check collision for two game objects 
	virtual bool CheckObjectCollision(const Object* pLeft, const Object* pRight) = 0;
	//Overloaded, to take just numbers for parameters of left rect
	virtual bool CheckObjectCollision(int posX, int posY, int height, int width, const Object* pRight) = 0;
	virtual void Update() = 0;

	virtual ~PhysicsSystem() { }
};