#include "MovementComponent.h"
#include "../../Object.h"
#include "../../Transform/Transform.h"

#include <math.h>

MovementComponent::MovementComponent(Object* pParent, float originalRotation) 
	: Component(pParent, ComponentTypes::g_kMovementComponent)
	, m_originalRotation(originalRotation)
	, m_movementState(MovementState::kStop)
	, m_lastClickPosX(0)
	, m_lastClickPosY(0)
	, m_destanationPointX(0.0f)
	, m_destanationPointY(0.0f)
{

}

void MovementComponent::MoveObjectToPosition(int posX, int posY)
{

	float curPosX = GetParentObject()->GetTransform()->GetPositionX();
	float curPosY = GetParentObject()->GetTransform()->GetPositionY();

	UpdateParentRotation(posX, posY);
	m_destanationPointX = static_cast<float>(posX) - curPosX;
	m_destanationPointY = static_cast<float>(posY) - curPosY;
}

void MovementComponent::UpdateParentPosition(int plusPosX, int plusPosY)
{
	float newPosX = GetParentObject()->GetTransform()->GetPositionX() + static_cast<float>(plusPosX);
	float newPosY = GetParentObject()->GetTransform()->GetPositionY() + static_cast<float>(plusPosY);
	GetParentObject()->GetTransform()->SetPosition(newPosX, newPosY);
}

void MovementComponent::OnMouseClick(int clickPosX, int clickPosY)
{
	m_lastClickPosX = clickPosX;
	m_lastClickPosY = clickPosY;
}

void MovementComponent::UpdateParentRotation(int lookX, int lookY)
{
	float xDiff = GetParentObject()->GetTransform()->GetPositionX() - static_cast<float>(lookX);
	float yDiff = GetParentObject()->GetTransform()->GetPositionY() - static_cast<float>(lookY);

	GetParentObject()->GetTransform()->SetRotation(atan2(yDiff, xDiff) * (180.0f / 3.14f));
}
