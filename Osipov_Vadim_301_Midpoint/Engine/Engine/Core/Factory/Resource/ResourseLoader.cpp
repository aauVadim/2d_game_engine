#include "ResourseLoader.h"

#include "../../Renderer/Virtuals/ITextureAtlas.h"
#include "../../Renderer/Implementation/SDL/SDL_TextureAtlas.h"
#include "../../Config.h"


std::string ResourseLoader::m_imageFileFormat = ".png";

IRendererDevice* ResourseLoader::m_pRenderDevice = nullptr;
IWindow* ResourseLoader::m_pWindow = nullptr;
std::vector<ITextureAtlas*> ResourseLoader::m_textureAtlases;
//Setting configs to none
Config* ResourseLoader::m_pConfigs = nullptr;

void ResourseLoader::CreateTextureAtlas(const PlatformConfiguration platforConfig, const std::string& name, int coluns, int firstGID, int imageHeight, int imageWidth, int margin, int spacing, int tileCount, int tileHeight, int tileWidth)
{
	//Making path
	std::string texturePath(GetImagesLocation() + name + m_imageFileFormat);

	switch (platforConfig)
	{
	case PlatformConfiguration::g_kSDL:
		m_textureAtlases.push_back(new SDL_TextureAtlas(texturePath, name, coluns, firstGID, imageHeight, imageWidth, margin, spacing, tileCount, tileHeight, tileWidth));
		break;
	case PlatformConfiguration::g_kDirectX:
		break;
	case PlatformConfiguration::g_kOpenGL:
		break;
	default:
		break;
	}
}

ITextureAtlas* ResourseLoader::GetTextureAtlasByGID(int gid)
{
	for (auto pAtlas : m_textureAtlases)
	{
		if (pAtlas->GetFirstGID() <= gid && gid <= pAtlas->GetLastGID())
			return pAtlas;
	}

	return nullptr;
}

void ResourseLoader::ShutDown()
{
	for (auto pAtlas : m_textureAtlases)
		delete pAtlas;

	m_textureAtlases.clear();

	delete m_pConfigs;
}

