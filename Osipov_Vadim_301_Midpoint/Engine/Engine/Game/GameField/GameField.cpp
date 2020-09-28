#include "GameField.h"
#include "Tile/Tile.h"


GameField::GameField(int fieldHeight, int fieldWidth)
	: m_fieldHeight(fieldHeight)
	, m_fieldWidth(fieldWidth)
	, m_tileHeight(0)
	, m_tileWidth(0)
	, m_groundTilesArraySize(fieldWidth*fieldHeight)
{
	m_groundTiles = new Tile*[m_groundTilesArraySize];

	for (int i = 0; i < (m_groundTilesArraySize); ++i)
		m_groundTiles[i] = nullptr;
}

GameField::~GameField()
{
	for (int i = 0; i < (m_groundTilesArraySize); ++i)
	{
		if (m_groundTiles[i] != nullptr)
			delete m_groundTiles[i];
	}

	delete[] m_groundTiles;
}

void GameField::InserTile(Tile* pTile, int index)
{
	if (m_groundTiles[index] == nullptr)
		m_groundTiles[index] = pTile;
	else
	{
		delete m_groundTiles[index];
		m_groundTiles[index] = pTile;
	}
}