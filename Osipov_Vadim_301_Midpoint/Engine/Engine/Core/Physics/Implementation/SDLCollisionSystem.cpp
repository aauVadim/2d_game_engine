#include "SDLCollisionSystem.h"
#include "../../../Game/Object/Object.h"
#include "../../../Game/Object/Transform/Transform.h"
#include <SDL_rect.h>

bool SDLCollisionSystem::CheckObjectCollision(const Object* pLeft, const Object* pRight)
{
	SDL_Rect leftRect, rightRect;

	Transform leftTrans = *pLeft->GetTransform();
	leftRect.x = static_cast<int>(leftTrans.GetPositionX());
	leftRect.y = static_cast<int>(leftTrans.GetPositionY());
	leftTrans.GetObjectScale(leftRect.w, leftRect.h);
	
	Transform rightTrans = *pRight->GetTransform();
	rightRect.x = static_cast<int>(rightTrans.GetPositionX());
	rightRect.y = static_cast<int>(rightTrans.GetPositionY());
	rightTrans.GetObjectScale(rightRect.w, rightRect.h);

	return SDL_HasIntersection(&leftRect, &rightRect) == SDL_TRUE;
}

bool SDLCollisionSystem::CheckObjectCollision(int posX, int posY, int height, int width, const Object* pRight)
{
	//throw std::logic_error("The method or operation is not implemented.");
	SDL_Rect leftRect, rightRect;

	leftRect.x = posX;
	leftRect.y = posY;
	leftRect.h = height;
	leftRect.w = width;

	Transform rightTrans = *pRight->GetTransform();
	rightRect.x = static_cast<int>(rightTrans.GetPositionX());
	rightRect.y = static_cast<int>(rightTrans.GetPositionY());
	rightTrans.GetObjectScale(rightRect.w, rightRect.h);

	return SDL_HasIntersection(&leftRect, &rightRect) == SDL_TRUE;

}
