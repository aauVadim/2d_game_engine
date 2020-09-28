#pragma once
//Interfaces
#include "../../Virtuals/IRendererDevice.h"
#include "../../../Engine/Event System/EventListener.h"

#include <vector>

struct SDL_Renderer;
class SDLWindow;
class SDLCamera;

class Object;

class GameField;
class Scene;
class Tile;


class SDLRenderer : public IRendererDevice, public EventListener
{
	SDL_Renderer* m_pRenderer;

	//Reference to Scene
	Scene* m_pSceneRef;

	//Reference to SDL Window class that we care for.
	SDLWindow* m_pWindowClassRef;
	//Reference to SDL Camera class
	SDLCamera* m_pCameraClassRef;

public:
	SDLRenderer();
	~SDLRenderer();

	//RendererDevice Virtual
	virtual void Update() override;

	//---------------------------------------------------------------------------------------------
	// Reference to Renderer
	//---------------------------------------------------------------------------------------------
	virtual IRendererDevice* GetRenderer() override { return (IRendererDevice*)m_pRenderer; };

	//---------------------------------------------------------------------------------------------
	// Creates renderer for given window
	//---------------------------------------------------------------------------------------------
	virtual bool CreateRenderDevice(IWindow* pWindow) override;
	
	// Inherited via EventListener
	//************************************
	// Method:    OnEvent
	// FullName:  SDLRenderer::OnEvent
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Parameter: const Event * pEvent
	//************************************
	virtual void OnEvent(const Event * pEvent) override;

private:
	void RenderGroundTiles();
	void RenderGameObjects();

	void AddObjectToRenderer(Object* pObj);
	void AddTileToRenderer(Tile* pTile);
};
