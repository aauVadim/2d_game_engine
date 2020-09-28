#pragma once

class Engine;

// Pure virtual class for any Window that is made by application
class IWindow
{
public:
	virtual ~IWindow() { };
	
	//---------------------------------------------------------------------------------------------
	// Initialize the Window Class, returns false if something went wrong
	//---------------------------------------------------------------------------------------------
	virtual bool Initialize() = 0;

	//---------------------------------------------------------------------------------------------
	// Return reference to current Window class
	//---------------------------------------------------------------------------------------------
	virtual IWindow* GetWindow() = 0;

	//---------------------------------------------------------------------------------------------
	// Should be called once per Tick for window to Update
	//---------------------------------------------------------------------------------------------
	virtual void Update() = 0;
};
