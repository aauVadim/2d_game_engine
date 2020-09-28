#pragma once

#include "../../Core/JSON/json_api.h"
#include "../../Core/Engine/Event System/EventListener.h"

#include "Tile/Tile.h"

//Should be read from TileD JSON File
class GameField
{
	// Letting this class know to trust json_api
	friend class json_api;

	//Height of the field in tiles
	int m_fieldWidth;
	//width of the field in tiles. 
	int m_fieldHeight;

	//Single tile height
	int m_tileHeight;
	//Single tile width
	int m_tileWidth;

	//Array of ground tiles because tiles will not be delete or highly modified throughout a game session, so we can keep it cheap
	Tile** m_groundTiles;
	int m_groundTilesArraySize;
public:
	GameField(int fieldHeight, int fieldWidth);
	~GameField();

	// -- Getters -- 
	int GetFieldHeight() { return m_fieldHeight; }
	int GetFieldWidth() { return m_fieldWidth; }
	int GetTileHeight() { return m_tileHeight; }
	int GetTileWidth() { return m_tileWidth; }
	int GetTilesArraySize() const { return m_groundTilesArraySize; }
	Tile* GetTileByIndex(int index) const { return m_groundTiles[index]; }

	//Inserting tile into the field
	void InserTile(Tile* pTile, int index);
};