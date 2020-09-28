#pragma once

//OLD
#include "../../Config.h"
#include "Config/Config.h"
#include <vector>
#include <string>

class ITextureAtlas;
class IRendererDevice;
class IWindow;
struct Config;

class ResourseLoader
{
	static Config* m_pConfigs; 
	//Collection of atlases we have
	static std::vector<ITextureAtlas*> m_textureAtlases;

	static std::string m_imageFileFormat; 

	static IRendererDevice* m_pRenderDevice;
	static IWindow* m_pWindow;

	PlatformConfiguration m_platformConfig; 

public:
	//---------------------------------------------------------------------------------------------
	// Used to create Texture Atlases 
	//---------------------------------------------------------------------------------------------
	static void CreateTextureAtlas(const PlatformConfiguration platforConfig, const std::string& name, int coluns, int firstGID, int imageHeight, int imageWidth, int margin, int spacing, int tileCount, int tileHeight, int tileWidth);
	static ITextureAtlas* GetTextureAtlasByGID(int gid);
	static void ShutDown();

	//Configs part.
	static void SetConfigs(Config* pConfig) { m_pConfigs = pConfig; }
	static std::string GetMapFile() { return std::string(m_pConfigs->m_medialRoot + m_pConfigs->m_mapFilesFolder + m_pConfigs->m_tiledMapFile); }
	static std::string GetImagesLocation() { return std::string(m_pConfigs->m_medialRoot + m_pConfigs->m_imagesFolder); }
	static std::string GetScriptsLocation() { return std::string(m_pConfigs->m_medialRoot + m_pConfigs->m_scriptFolder); }
};