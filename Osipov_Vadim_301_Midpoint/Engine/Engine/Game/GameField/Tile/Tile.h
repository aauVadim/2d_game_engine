#pragma once

#include "../../Object/Object.h" 

// Simple representation of tiles 
class Tile
{
	int m_imageIndex;
	int m_objIndex;
	int m_width, m_height;
	float m_posX, m_posY;
public:
	Tile(int imageIndex, int height, int width, float posX, float posY, int index);
	~Tile() { }

	//Indexes
	int GetObjectImageIndex() const { return m_imageIndex; }
	int GetObjectIndex() const { return m_objIndex; }

	// Position
	float GetPositionX() const { return m_posX; }
	float GetPositionY() const { return m_posY; }

	//Scale
	int GetHeight() const { return m_height; }
	int GetWidth() const { return m_width; }
};
