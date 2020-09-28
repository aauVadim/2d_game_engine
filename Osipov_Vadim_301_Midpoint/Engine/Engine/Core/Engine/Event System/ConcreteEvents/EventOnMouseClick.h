#pragma once
#include "../Event.h"
#include "../EventTypes.h"

class EventOnMouseClick : public Event
{
public:
	enum class MouseState
	{
		kPressed, 
		kReleased
	};

	enum class MouseButton
	{
		//Left Mouse Button
		kLMB, 
		//Right Mouse Button 
		kRMB,
		//Middle Mouse Button 
		kMMB
	};

private:
	int m_mouse_posX, m_mouse_posY;
	MouseButton m_button;
	MouseState m_state;

public:

	EventOnMouseClick(int mouse_posX, int mouse_posY, MouseState state, MouseButton button) : Event(EventID::g_kEventOnMouseClick), m_mouse_posX(mouse_posX), m_mouse_posY(mouse_posY), m_state(state), m_button(button) { }

	//---------------------------------------------------------------------------------------------
	// Returns Click position by reference
	//---------------------------------------------------------------------------------------------
	void GetClickPosition(int& inX, int& inY) { inX = m_mouse_posX; inY = m_mouse_posY; }

	//---------------------------------------------------------------------------------------------
	// Returns state of the button
	//---------------------------------------------------------------------------------------------
	MouseState GetState() const { return m_state; }

	//---------------------------------------------------------------------------------------------
	// Returns what button was clicked 
	//---------------------------------------------------------------------------------------------
	MouseButton GetButton() const { return m_button; }
};