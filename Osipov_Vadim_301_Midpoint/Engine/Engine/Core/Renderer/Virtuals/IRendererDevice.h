#pragma once

class IWindow;
class IGORenderer;
class GameField;

// Pure Virtual Class 
class IRendererDevice
{
public:
	virtual ~IRendererDevice() { };
	
	//---------------------------------------------------------------------------------------------
	// Should be called once per tick
	//---------------------------------------------------------------------------------------------
	virtual void Update() = 0;
	
	//---------------------------------------------------------------------------------------------
	// Return Reference to current Renderer
	//---------------------------------------------------------------------------------------------
	virtual IRendererDevice* GetRenderer() = 0;

	//---------------------------------------------------------------------------------------------
	// Creates Renderer Device, returns false is something goes wrong. 
	//---------------------------------------------------------------------------------------------
	virtual bool CreateRenderDevice(IWindow* pWindow) = 0;
};