#pragma once

class Transform
{
	//Storing original position of an object, do we needed it? Not sure yet. 
	float m_originalPosX, m_originalPosY;
	//Game Field Position - For field movement and field logic
	float m_posX, m_posY;			//Position
								// Camera Offset
	int m_width, m_height;		//Scale
	float m_rotation;			//Rotation
public:
	Transform(float posX, float posY, int height, int width, float rotation) : m_posX(posX), m_posY(posY),
		m_width(height), m_height(width),
		m_rotation(rotation) {
		m_originalPosX = m_posX; m_originalPosY = m_posY;
	} //All magically works.

	
	//------------------------------------------------------------------------------------------------
	// Pivot
	//------------------------------------------------------------------------------------------------
	void GetObjectPivot(float& x, float& y) const { x = m_posX + (static_cast<float>(m_width) / 2.0f); y = m_posY + (static_cast<float>(m_height) / 2.0f); }

	//------------------------------------------------------------------------------------------------
	// Position 
	//------------------------------------------------------------------------------------------------
	float GetPositionX() const { return m_posX; }
	float GetPositionY() const { return m_posY; }
	
	//Used to set objects position in the window. 
	void SetPosition(const float& posX, const float& posY) { m_posX = posX; m_posY = posY; }


	//------------------------------------------------------------------------------------------------
	// Scale
	//------------------------------------------------------------------------------------------------
	int GetObjectWidth() const { return m_width; }		//Single Width 
	int GetObjectHeight() const { return m_height; }	//Single Height
	// All Together, 
	void GetObjectScale(int& outWidth, int& outHeight) { outWidth = m_width; outHeight = m_height; }
	void SetScale(int scaleX, int scaleY) { m_width = scaleX; m_height = scaleY; }
	
	//------------------------------------------------------------------------------------------------
	// Rotation
	//------------------------------------------------------------------------------------------------
	float GetObjectRotation() const { return m_rotation; }
	void SetRotation(float rotation) { m_rotation = rotation; }
};
