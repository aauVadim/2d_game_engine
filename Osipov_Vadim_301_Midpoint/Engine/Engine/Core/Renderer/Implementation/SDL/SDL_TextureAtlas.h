#pragma once
#include <string>
#include "SDL.h"

#include "../../Virtuals/ITextureAtlas.h"

#include <unordered_map>

class SDL_TextureAtlas : public ITextureAtlas
{
	std::unordered_map<int, SDL_Rect*> m_tileRects;

	//Allowing 
	friend class ResourseLoader;

	//JSON Values
	int m_colums;
	int m_firstGID;
	int m_lastGID;
	int m_imageHeight;
	int m_imageWidth;
	int m_margin;
	int m_spacing;
	int m_tileCount;
	int m_tileHeight;
	int m_tileWidth;

	std::string m_imagePath;
	std::string m_imageName;


	SDL_Texture* m_pAtlasTexture = nullptr;

	//Return Values
	SDL_Texture* GetAtlasTexture() const { return m_pAtlasTexture; }

	// Inherited via ITextureAtlas
	virtual int GetFirstGID() const override { return m_firstGID; }
	virtual int GetLastGID() const override { return m_lastGID; }

	virtual int GetImageSpacing() const override { return m_spacing; }
	virtual int GetImageMargin() const override { return m_margin; }

public:
	SDL_TextureAtlas(std::string texturePath, std::string name, int coluns, int firstGID, int imageHeight, int imageWidth, int margin, int spacing, int tileCount, int tileHeight, int tileWidth);
	~SDL_TextureAtlas();

	SDL_Texture* GetTexture() const { return m_pAtlasTexture; }
	SDL_Rect* GetRectByGID(int gid);

private:
	SDL_Texture* LoadSDLTexture(std::string path);
	void GetTileRects();

};