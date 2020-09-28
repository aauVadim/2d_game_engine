#include "SDL_TextureAtlas.h"

#include "../../../Factory/Factory.h"
#include "../../../OS/Console/Logger.h"
#include "SDL.h"
#include "SDL_image.h"

SDL_TextureAtlas::SDL_TextureAtlas(std::string texturePath, std::string name, int coluns, int firstGID, int imageHeight, int imageWidth, int margin, int spacing, int tileCount, int tileHeight, int tileWidth)
	: m_imageName(name)
	, m_colums(coluns)
	, m_firstGID(firstGID)
	, m_lastGID(0)
	, m_imageHeight(imageHeight)
	, m_imageWidth(imageWidth)
	, m_margin(margin)
	, m_spacing(spacing)
	, m_tileCount(tileCount)
	, m_tileHeight(tileHeight)
	, m_tileWidth(tileWidth)
{
	//Loading its texture.
	m_pAtlasTexture = LoadSDLTexture(texturePath);
	//If we have the image, chop it for images.
	if (m_pAtlasTexture != nullptr)
		GetTileRects();
}

SDL_TextureAtlas::~SDL_TextureAtlas()
{
	for (auto pair : m_tileRects)
	{
		delete pair.second;
	}

	m_tileRects.clear();
}

SDL_Rect* SDL_TextureAtlas::GetRectByGID(int gid)
{
	//Safety bail
	if (gid == 0)
		return nullptr;

	return m_tileRects[gid];
}

SDL_Texture* SDL_TextureAtlas::LoadSDLTexture(std::string path)
{
	SDL_Surface* pSurface = IMG_LoadPNG_RW(SDL_RWFromFile(path.c_str(), "rb"));
	if (!pSurface)
	{
		Logger::GetInstance()->LogError("Failed to load: %s, Error: %s", path, SDL_GetError());
		return nullptr;
	}

	SDL_Texture* pTexture = SDL_CreateTextureFromSurface((SDL_Renderer*)Factory::m_pCurrentRenderer->GetRenderer(), pSurface);
	if (!pTexture)
	{
		Logger::GetInstance()->LogError("Failed to create texture: %s, Error: %s", path, SDL_GetError());
		return nullptr;
	}
	SDL_FreeSurface(pSurface);

	Logger::GetInstance()->Log("Texture atlas: ( %s ) is loaded", path.c_str());
	return pTexture;
}

void SDL_TextureAtlas::GetTileRects()
{
	int tilesInARow = m_imageWidth / m_tileWidth;
	int tilesInAColum = m_imageHeight / m_tileHeight;
	//Getting last tile number
	m_lastGID = tilesInARow * (m_imageHeight / m_tileHeight) + m_firstGID - 1;

	int tileNum = m_firstGID;
	for (int y = 0; y < tilesInAColum; ++y)
	{
		for (int x = 0; x < tilesInARow; ++x)
		{
			SDL_Rect* pRect = new SDL_Rect();
			pRect->x = ((x * m_tileWidth) + (m_spacing * x)) + m_margin;
			pRect->y = ((y * m_tileHeight) + (m_spacing * y)) + m_margin;
			pRect->h = m_tileHeight;
			pRect->w = m_tileWidth;
			m_tileRects.insert(std::make_pair(tileNum, pRect));
			tileNum++;
		}
	}
}
