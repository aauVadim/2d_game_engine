#pragma once
#include "../Event.h"
#include "../EventTypes.h"

class EventOnMarqueeSelect : public Event
{
	int m_posX, m_posY;
	int m_height, m_width;
public:
	//Returns a Rect 
	EventOnMarqueeSelect(int posX, int posY, int height, int width) : Event(EventID::g_kEventOnMarqueeSelect), m_posX(posX), m_posY(posY), m_height(height), m_width(width) { }

	//Returns Rect Data
	void GetBoxData(int& outPosX, int& outPosY, int& outHeight, int& outWidth) { outPosX = m_posX; outPosY = m_posY; outHeight = m_height; outWidth = m_width; }
};