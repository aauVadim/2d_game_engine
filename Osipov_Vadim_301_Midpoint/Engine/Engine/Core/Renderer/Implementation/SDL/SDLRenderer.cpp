//Vadim Osipov

#include "SDLRenderer.h"
#include <SDL.h>
#include <SDL_image.h>

#include "../../../UtilMacros.h"
#include "../../../OS/Window/Implementation/SDLWindow.h"
#include "../../../OS/Window/Implementation/Camera/SDLCamera.h"

#include "../../../../Game/Object/Object.h"
#include "../../../../Game/GameField/Tile/Tile.h"

#include "../../../../Game/GameField/Tile/Tile.h"
#include "../../../../Game/GameField/GameField.h"
#include "../../../../Game/Scene/Scene.h"
#include "../../../../Game/Object/Object.h"
#include "../../../../Game/Object/Transform/Transform.h"

#include "../../../Factory/Resource/ResourseLoader.h"
#include "SDL_TextureAtlas.h"

//Events
#include "../../../Engine/Event System/EventSystem.h"
#include "../../../Engine/Event System/EventTypes.h"
// Concrete events
#include "../../../Engine/Event System/ConcreteEvents/EventOnObjectCreated.h"
#include "../../../Engine/Event System/ConcreteEvents/EventOnGameFieldCreated.h"
#include "../../../Engine/Event System/ConcreteEvents/EventOnSceneCreated.h"

//Here we go with SDL Things
SDLRenderer::SDLRenderer()
	: m_pRenderer(nullptr)
	, m_pSceneRef(nullptr)
{
	EventSystem::AttachListener(EventID::g_kEventOnSceneCreated, this);
}

SDLRenderer::~SDLRenderer()
{
	SDL_DestroyRenderer(m_pRenderer);
}

void SDLRenderer::Update()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 0);
	SDL_RenderClear(m_pRenderer);

	//This logic was taking too much space, so I moved it to a function
	RenderGroundTiles();
	RenderGameObjects();

	SDL_RenderPresent(m_pRenderer);
}

bool SDLRenderer::CreateRenderDevice(IWindow* pWindow)
{
	//storing our window reference
	m_pWindowClassRef = (SDLWindow*)pWindow;
	//Storing our camera reference
	m_pCameraClassRef = m_pWindowClassRef->GetWindowCamera();

	m_pRenderer = SDL_CreateRenderer((SDL_Window*)m_pWindowClassRef->GetWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	return m_pRenderer != nullptr;
}

void SDLRenderer::OnEvent(const Event * pEvent)
{
	//Safe to assume that we've got OnSceneCreated event 
	if (pEvent->GetEngineEventId() == EventID::g_kEventOnSceneCreated)
	{
		m_pSceneRef = ((EventOnSceneCreated*)pEvent)->GetScene();
	}
}

void SDLRenderer::RenderGroundTiles()
{
	if (m_pSceneRef != nullptr && m_pSceneRef->GetGameField() != nullptr)
	{
		//Rendering tiles
		for (int i = 0; i < m_pSceneRef->GetGameField()->GetTilesArraySize(); ++i)
		{
			AddTileToRenderer(m_pSceneRef->GetGameField()->GetTileByIndex(i));
		}
	}
}

void SDLRenderer::RenderGameObjects()
{
	if (m_pSceneRef != nullptr)
	{
		for (auto pObj : m_pSceneRef->GetGameObjectMap())
		{
			AddObjectToRenderer(pObj.second);
			//This object has children
			if (pObj.second->GetChildrenVector().size() > 0)
			{
				//Making sure to render out children as well 
				for (Object* pChildGO : pObj.second->GetChildrenVector())
				{
					AddObjectToRenderer(pChildGO);
				}
			}
		}
	}
}



void SDLRenderer::AddObjectToRenderer(Object* pObj)
{
	SDL_Rect objRect;

	//Position
	//pObj->GetObjectTransform()->GetObjectFieldPosition(objRect.x, objRect.y);
	objRect.x = static_cast<int>(pObj->GetTransform()->GetPositionX() + m_pCameraClassRef->GetCameraOffsetX());
	objRect.y = static_cast<int>(pObj->GetTransform()->GetPositionY() + m_pCameraClassRef->GetCameraOffsetY());

	//Scale 
	pObj->GetTransform()->GetObjectScale(objRect.w, objRect.h);

	SDL_TextureAtlas* textureAtlas = (SDL_TextureAtlas*)ResourseLoader::GetTextureAtlasByGID(pObj->GetImageIndex());
	if (textureAtlas != nullptr)
	{
		SDL_Rect* pSourceRect = textureAtlas->GetRectByGID(pObj->GetImageIndex());
		if (pSourceRect != nullptr)
		{
			SDL_RenderCopyEx(
				m_pRenderer,
				textureAtlas->GetTexture(),
				pSourceRect,
				&objRect,
				pObj->GetTransform()->GetObjectRotation(),
				NULL, // Null because SDL will take middle of Destination Rect as pivot, no work needed. 
				SDL_FLIP_NONE
				);
		}
	}
	// Outlining the object
	if (pObj->IsSelected())
	{
		SDL_SetRenderDrawColor(m_pRenderer, 0, 225, 0, 0); // red 		
		SDL_RenderDrawRect(m_pRenderer, &objRect);
		SDL_SetRenderDrawColor(m_pRenderer, 225, 0, 0, 0); // red 		
	}

	//Marquee
	if (m_pCameraClassRef->GetMarqueeRect() != nullptr)
	{
		SDL_Rect pMarquee = *m_pCameraClassRef->GetMarqueeRect();
		SDL_SetRenderDrawColor(m_pRenderer, 225, 0, 0, 0); // green

		//Case where I store H and W as current mouse position, and X and Y as original 
		SDL_RenderDrawLine(m_pRenderer, pMarquee.x, pMarquee.y, pMarquee.w, pMarquee.y);
		SDL_RenderDrawLine(m_pRenderer, pMarquee.w, pMarquee.y, pMarquee.w, pMarquee.h);
		SDL_RenderDrawLine(m_pRenderer, pMarquee.w, pMarquee.h, pMarquee.x, pMarquee.h);
		SDL_RenderDrawLine(m_pRenderer, pMarquee.x, pMarquee.h, pMarquee.x, pMarquee.y);
	}
}

void SDLRenderer::AddTileToRenderer(Tile* pTile)
{
	SDL_Rect objRect;

	objRect.x = static_cast<int>(pTile->GetPositionX() + m_pCameraClassRef->GetCameraOffsetX());
	objRect.y = static_cast<int>(pTile->GetPositionY() + m_pCameraClassRef->GetCameraOffsetY());
	objRect.h = pTile->GetHeight();
	objRect.w = pTile->GetWidth();

	SDL_TextureAtlas* textureAtlas = (SDL_TextureAtlas*)ResourseLoader::GetTextureAtlasByGID(pTile->GetObjectImageIndex());
	if (textureAtlas != nullptr)
	{
		SDL_Rect* pSourceRect = textureAtlas->GetRectByGID(pTile->GetObjectImageIndex());
		if (pSourceRect != nullptr)
		{
			SDL_RenderCopyEx(
				m_pRenderer,
				textureAtlas->GetTexture(),
				pSourceRect,
				&objRect,
				0.0f,
				NULL, // Null because SDL will take middle of Destination Rect as pivot, no work needed. 
				SDL_FLIP_NONE
				);
		}
	}
}

