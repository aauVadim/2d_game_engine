#include "Box2DPhysics.h"

#include "../../Game/Object/Component/Component.h"
#include "../../Game/Object/Component/Concrete/MovementComponent.h"
#include "../../Game/Object/Object.h"
#include "../../Game/Object/Transform/Transform.h"

Box2DPhisycs::Box2DPhisycs()
{
	//Creating Physics World? Setting Gravity to pull onto X 
	m_pB2DWorld = new b2World(b2Vec2(-9.8f, 0));
}

Box2DPhisycs::~Box2DPhisycs()
{
	for (auto pComp : m_dynamicObjects)
	{
		delete pComp;
	}
	m_dynamicObjects.clear();

	if (m_pB2DWorld) delete m_pB2DWorld;
}

//Left it here because more things can come into adding object with mover
void Box2DPhisycs::AddGOWithMovementComponent(MovementComponent* pMovementComp)
{
	Transform parentTransform = *pMovementComp->GetParentObject()->GetTransform();
	//Creating new physics member
	Box2DComponent* pNewMember = new Box2DComponent();
	
	//Defining Body 
	pNewMember->m_obj2dBodyDef.type = b2_dynamicBody;
	int heightX, heightY; 
	//Defining Shape 
	pMovementComp->GetParentObject()->GetTransform()->GetObjectScale(heightX, heightY);
	pNewMember->m_b2PolygonShape.SetAsBox(static_cast<float32>(heightX), static_cast<float32>(heightY));
	//Defining Fixture(?)
	pNewMember->m_b2FixtureDef.shape = &pNewMember->m_b2PolygonShape;
	pNewMember->m_b2FixtureDef.density = 1.0f;
	//Creating dynamic body
	pNewMember->m_obj2dBodyDef.position.Set(parentTransform.GetPositionX(), parentTransform.GetPositionY());
	pNewMember->m_pObj2dBody = m_pB2DWorld->CreateBody(&pNewMember->m_obj2dBodyDef);
	pNewMember->m_pObj2dBody->CreateFixture(&pNewMember->m_b2FixtureDef);

	pNewMember->m_movementCompRef = pMovementComp;
	//Referencing back to component 
	pMovementComp->SetPhysicsSystem(this);
	m_dynamicObjects.push_back(pNewMember);
}

void Box2DPhisycs::Update()
{

	m_pB2DWorld->Step(1.0f/60.0f, 8, 3);
	if (!m_dynamicObjects.empty())
	{
		for (Box2DComponent* pBoxComp : m_dynamicObjects)
		{
			b2Vec2 testVec = pBoxComp->m_pObj2dBody->GetLinearVelocity();
			testVec.x = -5; 
			testVec.y = -15;
			pBoxComp->m_pObj2dBody->SetLinearVelocity(testVec);
			pBoxComp->m_movementCompRef->GetParentObject()->GetTransform()->SetPosition(pBoxComp->m_pObj2dBody->GetPosition().x, pBoxComp->m_pObj2dBody->GetPosition().y);
		}
	}
}

