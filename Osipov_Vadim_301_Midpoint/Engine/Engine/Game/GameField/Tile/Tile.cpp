#include "Tile.h"


Tile::Tile(int imageIndex, int height, int width, float posX, float posY, int index)
	: m_imageIndex(imageIndex)
	, m_objIndex(index)
	, m_height(height)
	, m_width(width)
	, m_posX(posX)
	, m_posY(posY)
{

}