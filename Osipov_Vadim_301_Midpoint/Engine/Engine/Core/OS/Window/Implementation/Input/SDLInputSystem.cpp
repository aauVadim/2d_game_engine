#include "SDLInputSystem.h"

#include "../../../../Engine/Event System/EventSystem.h"
#include "../../../../Engine/Event System/EventTypes.h"
#include "../../../../Engine/Event System/ConcreteEvents/EventOnMouseClick.h"
#include "../../../../Engine/Event System/ConcreteEvents/EventOnKeyboardInput.h"
#include "../../../Console/Logger.h"

SDLInputSystem::SDLInputSystem()
	: m_mouseState(MouseState::kReleased)
{

}

void SDLInputSystem::OnButtonDown(SDL_Event& inputEvent, const float& offsetX, const float& offsetY)
{
	//Simple state machine
	if (inputEvent.type == SDL_MOUSEBUTTONDOWN)
		ProcessMouseInput(inputEvent, offsetX, offsetY);
	else if (inputEvent.type == SDL_KEYDOWN)
		ProcessKeyboardInput(inputEvent);
}

void SDLInputSystem::OnButtonUp(SDL_Event& inputEvent, const float& offsetX /*= 0.0f*/, const float& offsetY /*= 0.0f*/)
{

	//Reverting offset
	int revertOffX = inputEvent.button.x + static_cast<int>(-offsetX);
	int revertOffY = inputEvent.button.y + static_cast<int>(-offsetY);


	Logger::GetInstance()->Log("+-----------------------------------------------------------------");
	Logger::GetInstance()->Log("|Released Event: Sending: Actual Click:\t Mouse click: X: %d, Y: %d", inputEvent.button.x, inputEvent.button.y);
	Logger::GetInstance()->Log("|Released Event: Sending: To Field Click:\t Mouse click: X: %d, Y: %d", revertOffX, revertOffY);
	Logger::GetInstance()->Log("+-----------------------------------------------------------------");

	if(inputEvent.button.button == SDL_BUTTON_LEFT)
		EventSystem::AddEventForExecution(new EventOnMouseClick(revertOffX, revertOffY, EventOnMouseClick::MouseState::kReleased, EventOnMouseClick::MouseButton::kLMB));
	else if (inputEvent.button.button == SDL_BUTTON_RIGHT)
		EventSystem::AddEventForExecution(new EventOnMouseClick(revertOffX, revertOffY, EventOnMouseClick::MouseState::kReleased, EventOnMouseClick::MouseButton::kRMB));
}

//If we are here it is very safe to assume that event IS mouse event, and process it as such 
void SDLInputSystem::ProcessMouseInput(SDL_Event& inputEvent, const float& offsetX, const float& offsetY)
{
	//Reverting offset
	int revertOffX = inputEvent.button.x + static_cast<int>(-offsetX);
	int revertOffY = inputEvent.button.y + static_cast<int>(-offsetY);

	Logger::GetInstance()->Log("+-----------------------------------------------------------------");
	Logger::GetInstance()->Log("|Pressed Event: Sending: Actual Click:\t Mouse click: X: %d, Y: %d", inputEvent.button.x, inputEvent.button.y);
	Logger::GetInstance()->Log("|Pressed Event: Sending: To Field Click:\t Mouse click: X: %d, Y: %d", revertOffX, revertOffY);
	Logger::GetInstance()->Log("+-----------------------------------------------------------------");

	if (inputEvent.button.button == SDL_BUTTON_LEFT)
		EventSystem::AddEventForExecution(new EventOnMouseClick(revertOffX, revertOffY, EventOnMouseClick::MouseState::kPressed, EventOnMouseClick::MouseButton::kLMB));
	else if (inputEvent.button.button == SDL_BUTTON_RIGHT)
		EventSystem::AddEventForExecution(new EventOnMouseClick(revertOffX, revertOffY, EventOnMouseClick::MouseState::kPressed, EventOnMouseClick::MouseButton::kRMB));
}

//If we are here it is very safe to assume that event IS keyboard event, and process it as such 
void SDLInputSystem::ProcessKeyboardInput(SDL_Event& inputEvent)
{
	Logger::GetInstance()->Log("+-----------------------------------------------------------------");
	Logger::GetInstance()->Log("|Sending out: Keyboard Input:\t %d", (int)inputEvent.key.keysym.sym);
	Logger::GetInstance()->Log("+-----------------------------------------------------------------");
	EventSystem::AddEventForExecution(new EventOnKeyboardInput((int)inputEvent.key.keysym.sym));
}