#include "SDLCamera.h"
#include <SDL_rect.h>
#include "../../../../Config.h"
#include "../../../../Engine/Event System/EventSystem.h"
#include "../../../../Engine/Event System/EventSystem.h"
#include "../../../../Engine/Event System/Event.h"
#include "../../../../Engine/Event System/ConcreteEvents/EventOnMouseClick.h"
#include "../../../../Engine/Event System/ConcreteEvents/EventOnMarqueeSelect.h"

SDLCamera::SDLCamera()
	: m_pCameraRect(new SDL_Rect)
	, m_pScreenRect(new SDL_Rect)
	, m_panSpeed(0.01f)	//TODO: Move to Config.h
	, m_canMoveCamera(false)
	, m_offsetX(0.0f)
	, m_offsetY(0.0f)
	, m_shouldMarqueeSelect(false)
	, m_isMarqueeActive(false)
	, m_marqueeStartX(0)
	, m_marqueeStartY(0)
	, m_pMarqueeRect(nullptr)
{
	//Setting up camera rect
	m_pCameraRect->x = g_kCameraRectSize;
	m_pCameraRect->y = g_kCameraRectSize;
	m_pCameraRect->h = g_kWindowHeight - g_kCameraRectSize * 2;
	m_pCameraRect->w = g_kWindowWidth - g_kCameraRectSize * 2;

	m_pScreenRect->x = 0;
	m_pScreenRect->y = 0;
	m_pScreenRect->h = g_kWindowHeight;
	m_pScreenRect->w = g_kWindowWidth;

	m_screenCenter.x = g_kWindowWidth / 2;
	m_screenCenter.y = g_kWindowHeight / 2;
	
	//Events:
	EventSystem::AttachListener(EventID::g_kEventOnMouseClick, this);
}

SDLCamera::~SDLCamera()
{
	if (m_pMarqueeRect != nullptr)
		delete m_pMarqueeRect;

	delete m_pCameraRect;
	delete m_pScreenRect;
}

void SDLCamera::UpdateWindowState(SDL_Event& event)
{
	m_canMoveCamera = 
			(	event.window.event == SDL_WINDOWEVENT_ENTER 
			||	event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED 
			||	event.window.event == SDL_WINDOWEVENT_EXPOSED );
}

void SDLCamera::UpdateCameraPosition()
{
	if (m_canMoveCamera)
	{
		SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
		//If mouse IS NOT in rect, start moving the camera. 
		if (!SDL_PointInRect(&m_mousePos, m_pCameraRect))
			MoveCamera(m_mousePos);

	}
}

void SDLCamera::OnEvent(const Event* pEvent)
{
	switch (pEvent->GetEngineEventId())
	{
	case EventID::g_kEventOnMouseClick:


		if (((EventOnMouseClick*)pEvent)->GetButton() == EventOnMouseClick::MouseButton::kLMB)
		{
			if (((EventOnMouseClick*)pEvent)->GetState() == EventOnMouseClick::MouseState::kPressed)
			{
				m_shouldMarqueeSelect = true;
				SDL_GetMouseState(&m_marqueeStartX, &m_marqueeStartY);
			}
			else if (((EventOnMouseClick*)pEvent)->GetState() == EventOnMouseClick::MouseState::kReleased)
			{
				//Nuking marquee
				m_shouldMarqueeSelect = false;
				//Checking if the point was not the same
				if ((m_pMarqueeRect->x != m_pMarqueeRect->w) & (m_pMarqueeRect->y != m_pMarqueeRect->h))
				{
					if (m_isMarqueeActive)
					{
						//Disabling Marquee
						m_isMarqueeActive = false;
						int posX = m_pMarqueeRect->x + static_cast<int>(-GetCameraOffsetX());
						int posY = m_pMarqueeRect->y + static_cast<int>(-GetCameraOffsetY());
						EventSystem::AddEventForExecution(new EventOnMarqueeSelect(posX, posY, (m_pMarqueeRect->h - m_pMarqueeRect->y), (m_pMarqueeRect->w - m_pMarqueeRect->x)));
						delete m_pMarqueeRect;
						m_pMarqueeRect = nullptr;
					}
				}
				else
				{
					int posY = m_pMarqueeRect->y + static_cast<int>(-GetCameraOffsetY());
					int posX = m_pMarqueeRect->x + static_cast<int>(-GetCameraOffsetX());
					EventSystem::AddEventForExecution(new EventOnMarqueeSelect(posX, posY, 1, 1));
				}
			}
		}
		break;
	default:
		break;
	}
}

SDL_Rect* SDLCamera::GetMarqueeRect()
{
	if (m_shouldMarqueeSelect)
	{
		if(m_pMarqueeRect == nullptr)
			m_pMarqueeRect = new SDL_Rect();

		m_pMarqueeRect->x = m_marqueeStartX;
		m_pMarqueeRect->y = m_marqueeStartY;
		m_pMarqueeRect->w = m_mousePos.x;
		m_pMarqueeRect->h = m_mousePos.y;
		m_isMarqueeActive = true;

		return m_pMarqueeRect;
	}
	else
		return nullptr;
}

void SDLCamera::MoveCamera(const SDL_Point& mousePos)
{
	m_offsetX += static_cast<float>(m_screenCenter.x - mousePos.x) * m_panSpeed;
	m_offsetY += static_cast<float>(m_screenCenter.y - mousePos.y) * m_panSpeed;
}
